#pragma once

#include <stdio.h>

#if defined(_WIN32)
# define snprintf _snprintf
#endif

#if defined(_WIN32)
int vasprintf(char **ptr, const char *fmt, va_list ap);
#endif
