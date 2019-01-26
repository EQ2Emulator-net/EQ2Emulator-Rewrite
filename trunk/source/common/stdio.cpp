#include "stdio.h"

#if defined(_WIN32)
# include <stdlib.h>
#endif

#if defined(_WIN32)
int vasprintf(char **ptr, const char *fmt, va_list ap) {
    int size;

    if ((size = _vscprintf(fmt, ap)) == -1) {
        *ptr = NULL;
        return -1;
    }

    if ((*ptr = (char *)malloc(size + 1)) == NULL)
        return -1;

    if ((size = vsprintf(*ptr, fmt, ap)) < 0) {
        free(*ptr);
        *ptr = NULL;
    }

    return size;
}
#endif
