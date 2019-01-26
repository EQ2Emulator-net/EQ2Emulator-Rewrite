#include <stdio.h>
#include "string.h"
#include <ctype.h>
#if defined(_WIN32)
# include <Windows.h>
# include <process.h>
#else
# include <unistd.h>
#endif
#include "util.h"

/**
* @brief Sleeps for the given number of milliseconds.
*
* @param milliseconds The number of milliseconds to sleep for.
*/
void SleepMS(unsigned int milliseconds) {
#if defined(_WIN32)
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

pid_t GetPID() {
#if defined(_WIN32)
    return _getpid();
#else
    return getpid();
#endif
}

bool IsUInt(const char *str) {
    const char *ptr = str;

    if (ptr == NULL)
        return false;

    while (*ptr != '\0') {
        if (!isdigit(*ptr))
            return false;
    }

    return true;
}

const char * GetElapsedTime(time_t seconds, char *dst, unsigned int size) {
    int count = 0;
    time_t units;

    if (seconds > (60*60*24*7)) {
        units = seconds / (60*60*24*7);
        count += snprintf(dst + count, size - count, "%lld week%s, ", (long long)units, units == 1 ? "" : "s");
        seconds -= units * (60*60*24*7);
    }

    if (seconds > (60*60*24)) {
        units = seconds / (60*60*24);
        count += snprintf(dst + count, size - count, "%lld day%s, ", (long long)units, units == 1 ? "" : "s");
        seconds -= units * (60*60*24);
    }

    if (seconds > (60*60)) {
        units = seconds / (60*60);
        count += snprintf(dst + count, size - count, "%lld hour%s, ", (long long)units, units == 1 ? "" : "s");
        seconds -= units * (60*60);
    }

    if (seconds > 60) {
        units = seconds / 60;
        count += snprintf(dst + count, size - count, "%lld minute%s, ", (long long)units, units == 1 ? "" : "s");
        seconds -= units * 60;
    }

    if (seconds > 0)
        count += snprintf(dst + count, size - count, "%lld second%s, ", (long long)seconds, seconds == 1 ? "" : "s");

    if (count == 0)
        strlcpy(dst, "No time elapsed", size);
    else
        dst[count - 2] = '\0';

    return dst;
        
        
}

/*
void CopyUInt8(char *buf, int *offset, uint8_t value) {
    memcpy(buf + *offset, &value, sizeof(value));
    *offset += sizeof(value);
}

void CopyUInt16(char *buf, int *offset, uint16_t value) {
    memcpy(buf + *offset, &value, sizeof(value));
    *offset += sizeof(value);
}

void CopyUInt32(char *buf, int *offset, uint32_t value) {
    memcpy(buf + *offset, &value, sizeof(value));
    *offset += sizeof(value);
}

void CopyString(char *buf, int *offset, const char *value, int size) {
    strlcpy(buf + *offset, value, size);
    *offset += size;
}

uint8_t ReadUInt8(const char *buf, int *offset) {
    uint8_t value;

    memcpy(&value, buf + *offset, sizeof(value));
    *offset += sizeof(value);

    return value;
}

uint16_t ReadUInt16(const char *buf, int *offset) {
    uint16_t value;

    memcpy(&value, buf + *offset, sizeof(value));
    *offset += sizeof(value);

    return value;
}

uint32_t ReadUInt32(const char *buf, int *offset) {
    uint32_t value;

    memcpy(&value, buf + *offset, sizeof(value));
    *offset += sizeof(value);

    return value;
}

void ReadString(char *value, int size, const char *buf, int *offset) {
    strlcpy(value, buf + *offset, size);
    *offset += size;
}
*/

void DumpBytes(const unsigned char *bytes, unsigned int len) {
    unsigned int i, j;

    for (i = 0; i < len; i += 16) {
        printf("%04X: ", i);

        for (j = 0; j < 16 && i + j < len; j++) {
            if (j == 7)
                printf("- ");
            printf("%02X ", bytes[i + j]);
        }

        for (; j < 16; j++) {
            if (j == 7)
                printf("  ");
            printf("   ");
        }

        for (j = 0; j < 16 && i + j < len; j++)
            printf("%c", isprint(bytes[i + j]) ? bytes[i + j] : '.');

        printf("\n");
    }
}

void DumpBytes(const char *bytes, unsigned int len)  {
    DumpBytes((const unsigned char *)bytes, len);
}

void PrintWelcomeHeader(const char *name, int version_major, int version_minor, const char *version_phase, const char *url) {
    printf("Module: %s, Version: %d.%d %s\n", name, version_major, version_minor, version_phase);
    printf("\nCopyright (C) 2016 EVEmulator. ALL RIGHTS RESERVED\n");
    printf("Proprietary and confidential\n");
    printf("Private use only\n");
    printf("You should not have this\n");
    printf("In fact, do not even look at it\n");
    printf("Close your eyes\n");
    printf("Turn away\n");
    printf("I MEAN IT!!!\n\n");
    printf(" Website     : %s \n\n", url);
}

void UpdateWindowTitle(const char *title) {
#if defined (_WIN32)
    SetConsoleTitle(title);
#else
    printf("\033]0;%s\007", title);
    fflush(stdout);
#endif
}
