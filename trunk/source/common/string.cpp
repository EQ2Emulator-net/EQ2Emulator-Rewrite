#include "stdafx.h"

#include "string.h"

size_t
strlcpy(char *dst, const char *src, size_t size) {
    char *d = dst;
    const char *s = src;
    size_t n = size;

    if (n != 0 && --n != 0) {
        do {
            if ((*d++ = *s++) == 0)
                break;
        }
        while (--n != 0);
    }

    if (n == 0) {
        if (size != 0)
            *d = '\0';
        while (*s++)
            ;
    }

    return(s - src - 1);
}