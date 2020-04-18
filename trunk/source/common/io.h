#pragma once

#include <stdio.h>

bool IODirectoryExists(const char *dir);
bool IOCreateDirectory(const char *dir);

long IOFileSize(const char *file);
long IOFileSize(FILE *f);