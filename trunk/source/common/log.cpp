#include "stdafx.h"
#include "log.h"
#include "../common/util.h"
#include "../common/io.h"
#include "../common/Mutex.h"


#if defined(_WIN32)
#define FMT "d"
#else
#define FMT "ld"
#endif

#define LOG_DIR "logs"

using namespace std;

typedef struct {
	char abbrev;
	char name[8];
	char name_lower[8];
} LogTypeString;

typedef struct {
	FILE *main;
	FILE *other[LOG_TYPE_INVALID];
} LogFiles;

typedef struct {
	LoggingSystem::LogFormat format;
	bool split;
	bool pid;
	char prefix[32];
} LogConfig;

LoggingSystem::Log LoggingSystem::logs[LOG_CATEGORY_COUNT][LOG_TYPE_COUNT];

static LogTypeString log_types[] = {
	{ 'I', "Info", "info" },
	{ 'E', "Error", "error" },
	{ 'W', "Warn", "warn" },
	{ 'D', "Debug", "debug" },
	{ 'T', "Trace", "trace" }
};

static QueuedLock stdout_stream_lock;

/*
* An array of string that map directly to the enum
* \link LogType \endlink. We can easily use the the enum value of the
* \link LogType \endlink to get the character that represents it in O(1) time.
*/
static const char *log_categories[] = {
	"Init", "Signal", "TCP", "Packet", "CPI", "IO", "Config", "Database", "Thread",
	"Net", "General", "Patch", "Console", "Rules", "Command", "Parser", "Chat", "Stats",
    "Client", "Mutex", "Zone", "Player", "World", "NPC", "Lua", "Item"
};
static LogFiles log_files;
static LogConfig config;

static int last_yday = -1;
static pid_t pid = GetPID();

namespace LoggingSystem {
	/*
	static char * LogCreateFileName(char *buf, int size, struct tm *tm) {
	if (config.pid) {
	snprintf(buf, size, "%s/%04d-%02d-%02d_vgo_%s_%d.%s", LOG_DIR, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, LOG_NAME, pid, config.format == LOG_FORMAT_XML ? "xml" : "log");
	return buf;
	}
	*/

	static void LogCloseFile(FILE **f) {
		if (*f != NULL) {
			if (config.format == LOG_FORMAT_XML)
				fprintf(*f, "</%s>", "</eq2emu>");
			fclose(*f);
			*f = NULL;
		}
	}

	static void LogOpenFile(FILE **f, const char *file, const char* openMode) {
		if ((*f = fopen(file, openMode)) == NULL) {
			fprintf(stderr, "Unable to open log file %s: %s\n", file, appStrError().c_str());
			return;
		}

		//If this is an XML file, make sure the root tags are written. Then seek to after the opening
		//root tag so we begin writting log messages in between the two tags.
		if (config.format == LOG_FORMAT_XML)
			fprintf(*f, "<%s>\n", "<eq2emu>");
	}

	static void ReOpenFile(FILE **f, const char *file, const char* openMode = "a") {
		LogCloseFile(f);
		LogOpenFile(f, file, openMode);
	}

	static void ReOpenLogs(struct tm *tm) {
		char file[FILENAME_MAX];
		int i;

		//make sure the logs directory exists
		if (!IODirectoryExists(LOG_DIR)) {
			if (!IOCreateDirectory(LOG_DIR)) {
				LogCloseAll();
				return;
			}
		}

		//close/open the main log file
		if (config.pid)
			snprintf(file, sizeof(file), "%s/%04d-%02d-%02d_%s_%d.%s", LOG_DIR, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, config.prefix, pid, config.format == LOG_FORMAT_XML ? "xml" : "log");
		else
			snprintf(file, sizeof(file), "%s/%04d-%02d-%02d_%s.%s", LOG_DIR, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, config.prefix, config.format == LOG_FORMAT_XML ? "xml" : "log");
		ReOpenFile(&log_files.main, file);

		//loop through and close/open the specific log files if we're splitting logs
		if (config.split) {
			for (i = 0; i < LOG_TYPE_COUNT; i++) {
				if (config.pid)
					snprintf(file, sizeof(file), "%s/%04d-%02d-%02d_%s_%s_%d.%s", LOG_DIR, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, config.prefix, log_types[i].name_lower, pid, config.format == LOG_FORMAT_XML ? "xml" : "log");
				else
					snprintf(file, sizeof(file), "%s/%04d-%02d-%02d_%s_%s.%s", LOG_DIR, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, config.prefix, log_types[i].name_lower, config.format == LOG_FORMAT_XML ? "xml" : "log");
				ReOpenFile(&log_files.other[i], file, i == LOG_TYPE_TRACE ? "w+" : "a");
			}
		}
	}

	static void LogSetConsoleColor(int color) {
#if defined(_WIN32)
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		if (handle == NULL || handle == INVALID_HANDLE_VALUE)
			return;

		SetConsoleTextAttribute(handle, color);
#else
		printf("\033[%d;%d;40m", color > 100 ? 1 : 0, color > 100 ? color - 100 : color);
#endif
	}

	LogType LogTypeByName(const char *name) {
		int i;

		for (i = 0; i < LOG_TYPE_COUNT; i++) {
			if (strcmp(log_types[i].name, name) == 0)
				return static_cast<LogType>(i);
		}

		return LOG_TYPE_INVALID;
	}

	LogCategory LogCategoryByName(const char *name) {
		int i;

		for (i = 0; i < LOG_CATEGORY_COUNT; i++) {
			if (strcmp(log_categories[i], name) == 0)
				return static_cast<LogCategory>(i);
		}

		return LOG_INVALID;
	}

	void LogSetFormat(LogFormat format) {
		config.format = format;
	}

	void LogSetSplit(bool split) {
		config.split = split;
	}

	void LogSetPID(bool pid) {
		config.pid = pid;
	}

	void LogSetPrefix(const char *prefix) {
		strncpy(config.prefix, prefix, sizeof(config.prefix));
	}

	void LogSet(LogCategory category, LogType type, bool on, int level, int output, int color) {
		logs[category][type].on = on;
		logs[category][type].level = level;
		logs[category][type].output = output;
		logs[category][type].color = color;
	}

	void LogStart() {
		LogCategory category;
		LogType type;
		int i, j;

#if defined(EQ2_DEBUG)
		if (sizeof(log_types) / sizeof(LogTypeString) != LOG_TYPE_COUNT) {
			fprintf(stderr, "**WARNING**WARNING**\n");
			fprintf(stderr, "The number of log types differ from the max number of log types allowed: Fix ASAP as a crash may occur\n");
			fprintf(stderr, "types (%u) != max categories (%u)\n", (unsigned int)(sizeof(log_types) / sizeof(LogTypeString)), LOG_TYPE_COUNT);
			fprintf(stderr, "**WARNING**\n");
		}
		if (sizeof(log_categories) / sizeof(const char *) != LOG_CATEGORY_COUNT) {
			fprintf(stderr, "**WARNING**WARNING**\n");
			fprintf(stderr, "The number of log categories differ from the max number of log categories allowed: Fix ASAP as a crash may occur\n");
			fprintf(stderr, "categories (%u) != max categories (%u)\n", (unsigned int)(sizeof(log_categories) / sizeof(const char *)), LOG_CATEGORY_COUNT);
			fprintf(stderr, "**WARNING**WARNING**\n");
		}
#endif

		log_files.main = NULL;
		for (i = 0; i < LOG_TYPE_COUNT; i++)
			log_files.other[i] = NULL;

		config.format = LOG_FORMAT_TEXT;
		config.split = false;
		config.pid = false;
		strncpy(config.prefix, "eq2emu", sizeof(config.prefix));

		for (i = 0; i < LOG_CATEGORY_COUNT; i++) {
			category = static_cast<LogCategory>(i);

			for (j = 0; j < LOG_TYPE_COUNT; j++) {
				type = static_cast<LogType>(j);

				switch (type) {
				case LOG_TYPE_INFO:
					LogSet(category, type, true, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_WHITE_BOLD);
					break;
				case LOG_TYPE_ERR:
					LogSet(category, type, true, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_RED_BOLD);
					break;
				case LOG_TYPE_WARN:
					LogSet(category, type, true, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_YELLOW_BOLD);
					break;
				case LOG_TYPE_DEBUG:
					LogSet(category, type, true, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_GREEN_BOLD);
					break;
				case LOG_TYPE_TRACE:
					LogSet(category, type, false, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_CYAN_BOLD);
					break;
				default:
					LogSet(category, type, false, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_WHITE_BOLD);
					break;
				}
			}
		}
	}

	void LogStop() {
		int i;

		LogCloseFile(&log_files.main);
		for (i = 0; i < LOG_TYPE_COUNT; i++)
			LogCloseFile(&log_files.other[i]);
	}

	void LogInit() {
		LogCategory category;
		LogType type;
		int i, j;

		//#if defined(VG_DEBUG)
		//    if (sizeof(log_types) / sizeof(LogTypeString) != LOG_TYPE_COUNT)
		//        fprintf(stderr, "The number of log types differ from the number of log type defines: a crash may occur\n");
		//    if (sizeof(log_categories) / sizeof(const char *) != LOG_CATEGORY_COUNT)
		//        fprintf(stderr, "The number of log categories differ from the number of log category defines: a crash may occur\n");
		//#endif

		log_files.main = NULL;
		for (i = 0; i < LOG_TYPE_COUNT; i++)
			log_files.other[i] = NULL;

		config.format = LOG_FORMAT_TEXT;
		config.split = false;
		config.pid = false;

		for (i = 0; i < LOG_CATEGORY_COUNT; i++) {
			category = static_cast<LogCategory>(i);

			for (j = 0; j < LOG_TYPE_COUNT; j++) {
				type = static_cast<LogType>(j);

				switch (type) {
				case LOG_TYPE_INFO:
					LogSet(category, type, true, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_WHITE_BOLD);
					break;
				case LOG_TYPE_ERR:
					LogSet(category, type, true, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_RED_BOLD);
					break;
				case LOG_TYPE_WARN:
					LogSet(category, type, true, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_YELLOW_BOLD);
					break;
				case LOG_TYPE_DEBUG:
					LogSet(category, type, false, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_GREEN_BOLD);
					break;
				case LOG_TYPE_TRACE:
					LogSet(category, type, false, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_CYAN_BOLD);
					break;
				default:
					LogSet(category, type, false, 0, LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE, LOG_WHITE_BOLD);
					break;
				}
			}
		}
	}

	void LogCloseAll() {
		int i;

		LogCloseFile(&log_files.main);
		for (i = 0; i < LOG_TYPE_COUNT; i++)
			LogCloseFile(&log_files.other[i]);
	}

	//This holds all of the info we need to perform a log entry
	struct QueuedLogWrite {
		LogType type;
		LogCategory category;
		const char* file;
		const char* function;
		int line;
		char* buf;
		int buf_size;
		Log* log;
		struct tm tm;
		timeval tv;
	};

	//This container holds a list of log entries to be written
	static queue<QueuedLogWrite> writing_queue;
	//This lock is used to protect our writing_queue
	static SpinLock writing_queue_lock;

#if defined(_WIN32)
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
	int gettimeofday(struct timeval *tv, struct timezone *tz) {
		unsigned __int64 tmp = 0;
		FILETIME ft;

		GetSystemTimeAsFileTime(&ft);

		tmp |= ft.dwHighDateTime;
		tmp <<= 32;
		tmp |= ft.dwLowDateTime;

		//Convert the file time to a unix timestamp.
		tmp /= 10;
		tmp -= DELTA_EPOCH_IN_MICROSECS;

		tv->tv_sec = (long)(tmp * 0.000001);
		tv->tv_usec = (long)(tmp % 1000000);

		return 0;
	}

	int vasprintf(char **ptr, const char *fmt, va_list ap) {
		int size;

		if ((size = _vscprintf(fmt, ap)) == -1) {
			*ptr = NULL;
			return -1;
		}

		if ((*ptr = (char *)malloc(size + 1)) == NULL)
			return -1;

		if ((size = vsprintf(*ptr, fmt, ap)) < 0) {
			free(*ptr);
			*ptr = NULL;
		}

		return size;
	}
#endif

	/**
	* @brief Queues Logging information to be written
	*
	* @details Logging info is sent to the writing_queue, where the LogWriting thread handles the actual IO work
	*
	* @param level The log level of the log output.
	* @param type The log type of the log out.
	* @param fmt The printf format specifier for the log text.
	**/
	void LogWrite(LogType type, LogCategory category, const char *file, const char *function, unsigned int line, const char *fmt, ...) {
		int count;
		char *buf;
		va_list ap;
		Log *log;

		if (type == LOG_TYPE_INVALID) {
			LogError(LOG_CONSOLE, 0, "LogWrite with invalid type called from %s line %u... fix this.", function, line);
			return;
		}

		log = &logs[category][type];

		va_start(ap, fmt);
		count = vasprintf(&buf, fmt, ap);
		va_end(ap);

		//We are going to queue this info up and let the writing thread take care of the IO work
		QueuedLogWrite this_write;
		this_write.buf = buf;
		this_write.buf_size = count;
		this_write.category = category;
		this_write.file = file;
		this_write.function = function;
		this_write.line = line;
		this_write.type = type;
		this_write.log = log;

		//Get the time of day as a unix timestamp and also its microseconds.
		gettimeofday(&this_write.tv, NULL);
		time_t sec = this_write.tv.tv_sec;
		this_write.tm = *localtime(&sec);

		writing_queue_lock.Lock();
		writing_queue.push(std::move(this_write));
		writing_queue_lock.Unlock();
		return;
	}

	QueuedLock* GetStdoutLock() {
		return &stdout_stream_lock;
	}

	//This function should only be used by one thread at a time
	//It performs the writing to our log files and console
	void LogWritingThread(bool* writing) {
		int size;
		char* message;
		QueuedLogWrite this_write;
		while (*writing || !writing_queue.empty()) {
			while (true) {
				writing_queue_lock.Lock();
				if (writing_queue.empty()) {
					writing_queue_lock.Unlock();
					break;
				}
				this_write = std::move(writing_queue.front());
				writing_queue.pop();
				writing_queue_lock.Unlock();

				//tm_yday stores the day of the year (0-365). we'll always store the last day of year so when it changes
				//we know we have to open up new log files
				if (last_yday != this_write.tm.tm_yday) {
					last_yday = this_write.tm.tm_yday;
					ReOpenLogs(&this_write.tm);
				}

				//If we're writing to a file, we'll create 1 message based on what format the log is being written in. We only need
				//to create the message once since even if we're writing to different log files (split=on) since the log format is global
				if (this_write.log->output & LOG_OUTPUT_FILE) {
					if (config.format == LOG_FORMAT_XML) {
						size = this_write.buf_size + FILENAME_MAX + 1000;
						message = (char*)malloc(size);

						if (!message) {
							continue;
						}
						snprintf(message, size, "<log timestamp=\"%ld\" category=\"%s\" file=\"%s\" function=\"%s\" line=\"%u\">![CDATA[%s]]</log>", this_write.tv.tv_sec + (this_write.tv.tv_usec / 1000), log_categories[this_write.category], this_write.file, this_write.function, this_write.line, this_write.buf);
					}
					else {
						size = this_write.buf_size + 100;
						message = (char*)malloc(size);
						if (!message) {
							continue;
						}
						snprintf(message, size, "%02d:%02d:%02d.%03" FMT " %c %-8s %s", this_write.tm.tm_hour, this_write.tm.tm_min, this_write.tm.tm_sec, this_write.tv.tv_usec / 1000, log_types[this_write.type].abbrev, log_categories[this_write.category], this_write.buf);
					}
					//Write to the main log file then to its specific log file.
					if (log_files.main != NULL) {
						fprintf(log_files.main, "%s\n", message);
						fflush(log_files.main);
					}
					if (config.split && log_files.other[this_write.type] != NULL) {
						fprintf(log_files.other[this_write.type], "%s\n", message);
						fflush(log_files.other[this_write.type]);
					}

					free(message);
				}

				//If we're writing to console, do so now
				if (this_write.log->output & LOG_OUTPUT_CONSOLE) {
					stdout_stream_lock.Lock();
					LogSetConsoleColor(LOG_WHITE_BOLD);
					printf("%02i:%02i:%02i.%03li ", this_write.tm.tm_hour, this_write.tm.tm_min, this_write.tm.tm_sec, this_write.tv.tv_usec / 1000);
					LogSetConsoleColor(this_write.log->color);
					printf("%c ", log_types[this_write.type].abbrev);
					LogSetConsoleColor(LOG_WHITE_BOLD);
					printf("%-8s ", log_categories[this_write.category]);
					LogSetConsoleColor(this_write.log->color);
					printf("%s\n", this_write.buf);
					LogSetConsoleColor(LOG_WHITE);
					fflush(stdout);
					stdout_stream_lock.Unlock();
				}
				free(this_write.buf);
			}
			SleepMS(300);
		}
	}

	void InitLogFiles() {
		memset(&log_files, 0, sizeof(log_files));
	}
};