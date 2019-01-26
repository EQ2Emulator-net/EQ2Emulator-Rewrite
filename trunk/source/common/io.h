#pragma once
/**
* @file IO.h
*
* @brief I/O specific functions
*
* @details I/O specific functions
*
* @author Scatman
* @date 09/01/13 (originally for VGOEmulator.net)
*
* @section License
*/

#include <stdio.h>

bool IODirectoryExists(const char *dir);
bool IOCreateDirectory(const char *dir);

long IOFileSize(const char *file);
long IOFileSize(FILE *f);