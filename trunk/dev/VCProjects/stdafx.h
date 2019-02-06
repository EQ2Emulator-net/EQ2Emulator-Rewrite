#pragma once

//Our included libs
#include <zlib.h>


//Containers
#include <unordered_map>
#include <map>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <string>


//Utility
#include <memory>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <stdio.h>
#include <mutex>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <cstring>
#include <time.h>
#include <atomic>
#include <ctype.h>
#include <iostream>

//OS Specific
#if defined(_WIN32)
#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <io.h>
#include <sys/timeb.h>
# include <process.h>
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
# include <unistd.h>
#include <arpa/inet.h>
#endif