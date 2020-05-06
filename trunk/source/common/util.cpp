#include "stdafx.h"

#include "string.h"
#include "util.h"
#include "../common/log.h"
#include <iomanip>
#include <random>
#include <utility>
#include <fstream>

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

#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)

void DumpBytes(const unsigned char* bytes, unsigned int size, const char* header) {
	std::ostringstream ss;
	if (header) {
		ss << header << "\n";
	}
	unsigned int i, j;
	for (i = 0; i < size; i += 16) {
		ss << std::setfill('0') << std::setw(4) << std::hex << i << std::setw(1) << ": ";

		//print hex chars
		for (j = 0; j < 16 && i + j < size; j++) {
			if (j == 8)
				ss << "- ";
			ss << std::setw(2) << static_cast<unsigned int>(bytes[i + j]) << std::setw(1) << ' ';
		}

		//fill any space up to the ascii if the line wasn't full and an extra space
		for (j = j; j < 16; j++) {
			if (j == 8)
				ss << "  ";
			ss << "   ";
		}
		ss << " ";

		//print the ascii now
		for (j = 0; j < 16 && i + j < size; j++) {
			if (IS_PRINTABLE(bytes[i + j]))
				ss << bytes[i + j];
			else
				ss << '.';
		}
		ss << std::endl;
	}

	LogDebug(LOG_PACKET, 0, "DumpBytes: \n%s", ss.str().c_str());
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

std::string appStrError(int err_num) {
	char errbuf[512];
	if (err_num == 0) {
		err_num = errno;
	}

#ifdef _WIN32
	if (strerror_s(errbuf, sizeof(errbuf), err_num) != 0) {
		return "appStrError : strerror_s unsuccessful!";
	}
#elif (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
	if (strerror_r(err_num, errbuf, sizeof(errbuf)) != 0) {
		return "appStrError : strerror_s unsuccessful!";
	}
#else
	return strerror_r(err_num, errbuf, sizeof(errbuf));
#endif

	return errbuf;
}

bool IsUnsignedInt(const char *str) {
	int i = 0;

	if (str[0] == '\0')
		return false;

	while (str[i] != '\0') {
		if (str[i] < 48 || str[i] > 57)
			return false;
		++i;
	}

	return true;
}

bool alpha_check(unsigned char val) {
	if ((val >= 0x41 && val <= 0x5A) || (val >= 0x61 && val <= 0x7A))
		return true;
	else
		return false;
}

/**
* @brief constructs and returns the mt19937 object for this thread
*
* @author theFoof
* @date 6 Mar 2019
*/
std::mt19937& GetRandMT() {
	thread_local std::mt19937 mt(std::random_device().operator()());
	return mt;
}

/**
* @brief function to generate a random float
*
* @author Xinux
* @date 9 Jan 2015
*/
float MakeRandom(float low, float high) {
	if (low > high) {
		std::swap(low, high);
	}
	std::uniform_real_distribution<float> dist(low, high);

	return dist(GetRandMT());
}

/**
* @brief function to generate a random int
*
* @author theFoof
* @date 1 July 2016
*/
int32_t MakeRandomInt(int32_t low, int32_t high) {
	if (low > high) {
		std::swap(low, high);
	}
	std::uniform_int_distribution<int32_t> dist(low, high);

	return dist(GetRandMT());
}

uint32_t MakeRandomNumber() {
	std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

	return dist(GetRandMT());
}

bool GetFileAsString(std::string& out, const char* file_name) {
	std::ifstream reader(file_name);
	if (!reader)
		return false;

	std::ostringstream bufstream;
	bufstream << reader.rdbuf();
	out = std::move(bufstream.str());
	return true;
}