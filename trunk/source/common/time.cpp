#include "stdafx.h"

#include "time.h"

#if defined(_WIN32)
//# if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
//#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
//# else
//# if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
//#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
//# else
#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
//# endif
#else
# define DELTA_EPOCH_IN_SECONDS 11644473600LL
# define SECS_TO_100NS          10000000
#endif

#if defined(_WIN32)
/**
* @brief Obtains the current time expressed in seconds and microseconds.
*
* @details This function is only included if compiled on Windows since an
* implementation exists on Linux. The `tz' parameter should always
* be set to NULL since its obsolete, just as it is on Linux.
*
* @param tv A timeval structure to store seconds and microseconds.
* @param tz A timezone structure. Set to NULL.
* @return 0.
*/
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

struct tm * localtime_r(const time_t *timep, struct tm *result) {
    localtime_s(result, timep);
    return result;
}

struct tm * gmtime_r(const time_t *timep, struct tm *result) {
    gmtime_s(result, timep);
    return result;
}
#endif

unsigned long TimeMS() {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (unsigned long)((tv.tv_sec * 1000) + (tv.tv_usec * 0.001));
}

#if !defined(_WIN32)
static uint64_t UnixToWin32Time(time_t sec, int nano_sec) {
    return ((uint64_t)sec + DELTA_EPOCH_IN_SECONDS) * SECS_TO_100NS + (nano_sec / 100);
}
#endif

uint64_t TimeWin32() {
#if defined(_WIN32)
    FILETIME ft;

    GetSystemTimeAsFileTime(&ft);
    return (uint64_t)ft.dwHighDateTime << 32 | (uint64_t)ft.dwLowDateTime;
#else
    return UnixToWin32Time(time(NULL), 0);
#endif
}
