#include "stdafx.h"

#include "io.h"

bool IODirectoryExists(const char *dir) {
#if defined(_WIN32)
    DWORD attr = GetFileAttributes(dir);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;

    return (attr & FILE_ATTRIBUTE_DIRECTORY) > 0;
#else
    struct stat st;

    if (stat(dir, &st) != 0)
        return false;

    return S_ISDIR(st.st_mode);
#endif
}

bool IOCreateDirectory(const char *dir) {
#if defined(_WIN32)
    if (!CreateDirectory(dir, NULL))
        return false;
#else
    if (mkdir(dir, 0755) != 0)
        return false;
#endif

    return true;
}

long IOFileSize(FILE *f) {
    long size;

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    return size;
}

long IOFileSize(const char *file) {
#if defined(_WIN32)
    HANDLE handle = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    LARGE_INTEGER size;

    if (handle == INVALID_HANDLE_VALUE)
        return -1;

    if (!GetFileSizeEx(&handle, &size))
        return -1;

    return (long)size.QuadPart;
#else
    struct stat st;

    if (stat(file, &st) != 0)
        return -1;

    return st.st_size;
#endif
}