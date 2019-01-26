#pragma once

#include <stdint.h>
#include <time.h>
#if defined(_WIN32)
# include <WinSock2.h> //struct timeval
#else
# include <sys/time.h>
#endif

#if defined(_WIN32)
//struct timeval {
    //time_t tv_sec;
    //long int tv_usec;
//};

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int gettimeofday(struct timeval *tv, struct timezone *tz);
struct tm * localtime_r(const time_t *timep, struct tm *result);
struct tm * gmtime_r(const time_t *timep, struct tm *result);
#endif

unsigned long TimeMS();
uint64_t TimeWin32();
