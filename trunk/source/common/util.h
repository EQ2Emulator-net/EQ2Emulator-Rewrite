#pragma once

#include <stdint.h>
#include <time.h>
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

/*
void CopyUInt8(char *buf, int *offset, uint8_t value);
void CopyUInt16(char *buf, int *offset, uint16_t value);
void CopyUInt32(char *buf, int *offset, uint32_t value);
void CopyString(char *buf, int *offset, const char *value, int size);

uint8_t ReadUInt8(const char *buf, int *offset);
uint16_t ReadUInt16(const char *buf, int *offset);
uint32_t ReadUInt32(const char *buf, int *offset);
void ReadString(char *value, int size, const char *buf, int *offset);
*/

//TODO: move this to some game time module or something
const char * GetElapsedTime(time_t seconds, char *dst, unsigned int size);

void DumpBytes(const unsigned char *bytes, unsigned int len);
void DumpBytes(const char *bytes, unsigned int len);

void PrintWelcomeHeader(const char *name, int version_major, int version_minor, const char *version_phase, const char *url);
void UpdateWindowTitle(const char *title);
