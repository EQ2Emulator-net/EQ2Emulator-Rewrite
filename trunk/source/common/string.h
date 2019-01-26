#pragma once

#include <string.h>

#if defined(_WIN32)
# define strcasecmp _stricmp
# define strdup     _strdup
#endif

size_t strlcpy(char *dst, const char *src, size_t size);