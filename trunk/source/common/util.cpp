#include <stdio.h>
#include "string.h"
#include <ctype.h>
#include <zlib.h>
#include <iostream>
#include <map>

#if defined(_WIN32)
	# include <Windows.h>
	# include <process.h>
#else
	# include <unistd.h>
#endif

#include "util.h"

extern std::map<uint16_t, uint16_t> EQOpcodeVersions;

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

int Deflate(unsigned char* in_data, int in_length, unsigned char* out_data, int max_out_length) {
	z_stream zstream;
	int zerror;

	zstream.next_in = in_data;
	zstream.avail_in = in_length;
	zstream.zalloc = Z_NULL;
	zstream.zfree = Z_NULL;
	zstream.opaque = Z_NULL;
	deflateInit(&zstream, Z_FINISH);
	zstream.next_out = out_data;
	zstream.avail_out = max_out_length;
	zerror = deflate(&zstream, Z_FINISH);

	if (zerror == Z_STREAM_END) {
		deflateEnd(&zstream);
		return zstream.total_out;
	}
	else {
		std::cout << "Error: Deflate: deflate() returned " << zerror << " '";
		if (zstream.msg)
			std::cout << zstream.msg;
		std::cout << "'" << std::endl;
		zerror = deflateEnd(&zstream);
		return 0;
	}
}

int Inflate(unsigned char* indata, int indatalen, unsigned char* outdata, int outdatalen, bool iQuiet) {
	z_stream zstream;
	int zerror = 0;
	int i;

	zstream.next_in = indata;
	zstream.avail_in = indatalen;
	zstream.next_out = outdata;
	zstream.avail_out = outdatalen;
	zstream.zalloc = Z_NULL;
	zstream.zfree = Z_NULL;
	zstream.opaque = Z_NULL;

	i = inflateInit2(&zstream, 15);
	if (i != Z_OK) {
		return 0;
	}

	zerror = inflate(&zstream, Z_FINISH);

	if (zerror == Z_STREAM_END) {
		inflateEnd(&zstream);
		return zstream.total_out;
	}
	else {
		if (!iQuiet) {
			std::cout << "Error: Inflate: inflate() returned " << zerror << " '";
			if (zstream.msg)
				std::cout << zstream.msg;
			std::cout << "'" << std::endl;
		}

		if (zerror == -4 && zstream.msg == 0) {
			return 0;
		}

		zerror = inflateEnd(&zstream);
		return 0;
	}
}