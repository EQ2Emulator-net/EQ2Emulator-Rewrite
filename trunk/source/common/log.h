#pragma once

/**
* These macros are wrappers around LogWrite so that certain versions can be
* no-oped. The format is Log[level][debug level]. Any macro that has a debug
* level number higher than `DEBUG_LEVEL' will be no-oped out so that the
* function call is not even compiled in.
*
* Note: a ## before __VA_ARGS__ is required in GNU because if you don't supply
* any variadic arguments, the trailing comma is left. This is an extension
* that was put in just for this.
* eg. LogWarn(LOG_INIT, "hey"); would expand into:
*     LogWrite(LOG_LEVEL_WARN, LOG_INIT, "hey", );
*
*/

#define LogInfo(category, level, fmt, ...) \
	do {if (LoggingSystem::LogLevelCheck(category, LOG_TYPE_INFO, level))  \
		LoggingSystem::LogWrite(LOG_TYPE_INFO, category, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); } while(0)
#define LogError(category, level, fmt, ...) \
	do { if (LoggingSystem::LogLevelCheck(category, LOG_TYPE_ERR, level)) \
		LoggingSystem::LogWrite(LOG_TYPE_ERR, category, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); } while(0)
#define LogWarn(category, level, fmt, ...)  \
	do { if (LoggingSystem::LogLevelCheck(category, LOG_TYPE_WARN, level)) \
		LoggingSystem::LogWrite(LOG_TYPE_WARN, category, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); } while(0)
#define LogDebug(category, level, fmt, ...) \
	do { if (LoggingSystem::LogLevelCheck(category, LOG_TYPE_DEBUG, level)) \
		LoggingSystem::LogWrite(LOG_TYPE_DEBUG, category,  __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); } while(0)
#define LogTrace(category, level, fmt, ...) \
	do { if (LoggingSystem::LogLevelCheck(category, LOG_TYPE_TRACE, level)) \
		LoggingSystem::LogWrite(LOG_TYPE_TRACE, category,  __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); } while(0)

class QueuedLock;

typedef enum {
	LOG_INIT,
	LOG_SIGNAL,
	LOG_TCP,
	LOG_PACKET,
	LOG_CPI,
	LOG_IO,
	LOG_CONFIG,
	LOG_DATABASE,
	LOG_THREAD,
	LOG_NET,
	LOG_GENERAL,
	LOG_PATCH,
	LOG_CONSOLE,
	LOG_RULES,
	LOG_COMMAND,
	LOG_PARSER,
	LOG_CHAT,
	LOG_STATS,
	LOG_CLIENT,
	LOG_MUTEX,
	LOG_ZONE,
	LOG_PLAYER,
	LOG_WORLD,
	LOG_NPC,
	LOG_LUA,
	LOG_INVALID
} LogCategory;

typedef enum {
	LOG_TYPE_INFO,
	LOG_TYPE_ERR,
	LOG_TYPE_WARN,
	LOG_TYPE_DEBUG,
	LOG_TYPE_TRACE,
	LOG_TYPE_INVALID
} LogType;

namespace LoggingSystem {
#define LOG_OUTPUT_CONSOLE 1
#define LOG_OUTPUT_FILE    2
#define LOG_OUTPUT_WORLD   4

#if defined (_WIN32)
#define LOG_WHITE        (FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE)
#define LOG_WHITE_BOLD   (FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define LOG_RED          (FOREGROUND_RED)
#define LOG_RED_BOLD     (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define LOG_GREEN        (FOREGROUND_GREEN)
#define LOG_GREEN_BOLD   (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define LOG_BLUE         (FOREGROUND_BLUE)
#define LOG_BLUE_BOLD    (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define LOG_YELLOW       (FOREGROUND_RED | FOREGROUND_GREEN)
#define LOG_YELLOW_BOLD  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define LOG_CYAN         (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define LOG_CYAN_BOLD    (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define LOG_MAGENTA      (FOREGROUND_RED | FOREGROUND_BLUE)
#define LOG_MAGENTA_BOLD (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#else
#define LOG_WHITE        37
#define LOG_WHITE_BOLD   137
#define LOG_RED          31
#define LOG_RED_BOLD     131
#define LOG_GREEN        32
#define LOG_GREEN_BOLD   132
#define LOG_BLUE         34
#define LOG_BLUE_BOLD    134
#define LOG_YELLOW       33
#define LOG_YELLOW_BOLD  133
#define LOG_CYAN         36
#define LOG_CYAN_BOLD    136
#define LOG_MAGENTA      35
#define LOG_MAGENTA_BOLD 135
#endif

	typedef enum {
		LOG_FORMAT_TEXT,
		LOG_FORMAT_XML
	} LogFormat;

	LogType LogTypeByName(const char *name);
	LogCategory LogCategoryByName(const char *name);

	void LogSetFormat(LogFormat format);
	void LogSetSplit(bool split);
	void LogSetPID(bool pid);
	void LogSet(LogCategory category, LogType type, bool on, int level, int output, int color);

	void LogInit();
	void LogCloseAll();
	void LogStart();
	void LogStop();
	void LogSetPrefix(const char* prefix);
	void LogWrite(LogType type, LogCategory category, const char *file, const char *function, unsigned int line, const char *fmt, ...);
	QueuedLock* GetStdoutLock();

	//This thread is in charge of actually writing to the console/log files
	void LogWritingThread(bool* writing);
	void InitLogFiles();

	struct Log {
		bool on;
		int level;
		int output;
		int color;
	};

#define LOG_TYPE_COUNT     LOG_TYPE_INVALID
#define LOG_CATEGORY_COUNT LOG_INVALID

	extern Log logs[LOG_CATEGORY_COUNT][LOG_TYPE_COUNT];

	inline bool LogLevelCheck(LogCategory category, LogType type, int level) {
		Log* log = &logs[category][type];
		return (log->on && log->level >= level);
	}
};
