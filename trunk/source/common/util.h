#pragma once

#include <stdint.h>
#include <time.h>
#include <string>
#if defined(_WIN32)
#else
# include <unistd.h>
#endif

#if defined(_WIN32)
typedef int pid_t;
#endif

void SleepMS(unsigned int milliseconds);
pid_t GetPID();

bool IsUInt(const char *str);

//TODO: move this to some game time module or something
const char * GetElapsedTime(time_t seconds, char *dst, unsigned int size);

void DumpBytes(const unsigned char *bytes, unsigned int len);
void DumpBytes(const char *bytes, unsigned int len);

void PrintWelcomeHeader(const char *name, int version_major, int version_minor, const char *version_phase, const char *url);
void UpdateWindowTitle(const char *title);

int Deflate(unsigned char* in_data, int in_length, unsigned char* out_data, int max_out_length);
int Inflate(unsigned char* indata, int indatalen, unsigned char* outdata, int outdatalen, bool iQuiet = true);

std::string appStrError(int err_num = 0);
bool IsUnsignedInt(const char *str);

#define atoul(str) strtoul(str, NULL, 10)