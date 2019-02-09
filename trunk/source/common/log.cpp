#include "stdafx.h"

#include <stdlib.h>
#include "stdio.h"
#include <stdarg.h>
#include "string.h"
#include "time.h"
#include <errno.h>
#include "util.h"
#include "io.h"
#include "log.h"

#if defined(_WIN32)
# define FMT "d"
#else
# define FMT "ld"
#endif

#define LOG_DIR "logs"

#define LOG_TYPE_COUNT     LOG_TYPE_INVALID
#define LOG_CATEGORY_COUNT LOG_INVALID

typedef struct {
	bool on;
	int level;
	int output;
	int color;
} Log;

typedef struct {
	char abbrev;
	char name[8];
	char name_lower[8];
} LogTypeString;

typedef struct {
	FILE *main;
	FILE *other[LOG_TYPE_COUNT];
} LogFiles;

typedef struct {
	LogFormat format;
	bool split;
	bool pid;
    char prefix[16];
} LogConfig;

static Log logs[LOG_CATEGORY_COUNT][LOG_TYPE_COUNT];

static LogTypeString log_types[] = {
	{'I', "Info",  "info"},
	{'E', "Error", "error"},
	{'W', "Warn",  "warn"},
	{'D', "Debug", "debug"},
	{'T', "Trace", "trace"}
};

/*
* An array of string that map directly to the enum
* \link LogType \endlink. We can easily use the the enum value of the
* \link LogType \endlink to get the character that represents it in O(1) time.
*/
static const char *log_categories[] = {
	"Init", "Signal", "TCP", "Packet", "CPI", "IO", "Config", "Database", "Thread",
	"Net", "General", "Patch", "Console", "Rules", "Command", "Parser", "Chat", "Stats",
    "Client", "Mutex"
};
static LogFiles log_files;
static LogConfig config;

static int last_yday = -1;
static pid_t pid = GetPID();

static void LogCloseFile(FILE **f) {
	if (*f != NULL) {
		if (config.format == LOG_FORMAT_XML)
			fprintf(*f, "</eq2emu>");
		fclose(*f);
		*f = NULL;
	}
}

static void LogOpenFile(FILE **f, const char *file) {
	if ((*f = fopen(file, "a")) == NULL) {
		fprintf(stderr, "Unable to open log file %s: %s\n", file, strerror(errno));
		return;
	}

	//If this is an XML file, make sure the root tags are written. Then seek to after the opening
	//root tag so we begin writting log messages in between the two tags.
	if (config.format == LOG_FORMAT_XML)
		fprintf(*f, "<eq2emu>\n");
}

static void ReOpenFile(FILE **f, const char *file) {
	LogCloseFile(f);
	LogOpenFile(f, file);
}

static void ReOpenLogs(struct tm *tm) {
	char file[FILENAME_MAX];
	int i;

	//make sure the logs directory exists
	if (!IODirectoryExists(LOG_DIR)) {
		if (!IOCreateDirectory(LOG_DIR)) {
			LogStop();
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
			ReOpenFile(&log_files.other[i], file);
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
    strlcpy(config.prefix, prefix, sizeof(config.prefix));
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
    strlcpy(config.prefix, "eq2emu", sizeof(config.prefix));

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
			case LOG_TYPE_DEBUG: // TODO: Once server stabilizes, set this back to false by default
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

/**
* @brief Writes log output.
*
* @details WTF
*
* @param level The log level of the log output.
* @param type The log type of the log out.
* @param fmt The printf format specifier for the log text.
*/
void LogWrite(LogType type, LogCategory category, int level, const char *file, const char *function, unsigned int line, const char *fmt, ...) {
	int count, size;
	char *buf, *message;
	struct timeval tv;
	struct tm tm;
	time_t sec;
	va_list ap;
	Log *log;

	log = &logs[category][type];

	if (!log->on)
		return;
	if (level > 0 && log->level < level)
		return;

	va_start(ap, fmt);
	count = vasprintf(&buf, fmt, ap);
	va_end(ap);

	if (buf == NULL) {
		fprintf(stderr, "Out of memory trying to allocate log buffer (level=%d, file='%s', function='%s', line=%u)\n", level, file, function, line);
		return;
	}

	//Get the time of day as a unix timestamp and also its microseconds.
	gettimeofday(&tv, NULL);
	sec = tv.tv_sec;
	localtime_r(&sec, &tm);

	//tm_yday stores the day of the year (0-365). we'll always store the last day of year so when it changes
	//we know we have to open up new log files
	if (last_yday != tm.tm_yday) {
		last_yday = tm.tm_yday;
		ReOpenLogs(&tm);
	}

	//If we're writing to a file, we'll create 1 message based on what format the log is being written in. We only need
	//to create the message once since even if we're writing to different log files (split=on) since the log format is global
	if (log->output & LOG_OUTPUT_FILE) {
		if (config.format == LOG_FORMAT_XML) {
			size = count + FILENAME_MAX + 1000;
			if ((message = (char *)malloc(size)) == NULL) {
				free(buf);
				fprintf(stderr, "Out of memory trying to allocate %d bytes for log message", size);
				return;
			}

			snprintf(message, size, "<log timestamp=\"%ld\" category=\"%s\" file=\"%s\" function=\"%s\" line=\"%u\">![CDATA[%s]]</log>", (long)tv.tv_sec + (tv.tv_usec / 1000), log_categories[category], file, function, line, buf);
		}
		else {
			size = count + 100;
			if ((message = (char *)malloc(size)) == NULL) {
				free(buf);
				fprintf(stderr, "Out of memory trying to allocate %d bytes for log message", size);
				return;
			}

			snprintf(message, size, "%02d:%02d:%02d.%03" FMT " %c %-8s %s", tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec / 1000, log_types[type].abbrev, log_categories[category], buf);
		}

		//Write to the main log file then do its specific log file.
		if (log_files.main != NULL) {
			fprintf(log_files.main, "%s\n", message);
			fflush(log_files.main);
		}
		if (config.split && log_files.other[type] != NULL) {
			fprintf(log_files.other[type], "%s\n", message);
			fflush(log_files.other[type]);
		}

		free(message);
	}

	//If we're writing to console, do so now
	if (log->output & LOG_OUTPUT_CONSOLE) {
		LogSetConsoleColor(LOG_WHITE_BOLD);
		printf("%02d:%02d:%02d.%03" FMT " ", tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec / 1000);
		LogSetConsoleColor(log->color);
		printf("%c ", log_types[type].abbrev);
		LogSetConsoleColor(LOG_WHITE_BOLD);
		printf("%-8s ", log_categories[category]);
		LogSetConsoleColor(log->color);
		printf("%s\n", buf);
		LogSetConsoleColor(LOG_WHITE);
		fflush(stdout);
	}

	free(buf);
}