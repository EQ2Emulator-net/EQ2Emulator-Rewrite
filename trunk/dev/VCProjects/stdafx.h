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
#include <assert.h>
#include <chrono>
#include <cmath>
#include <optional>

//OS Specific
#if defined(_WIN32)
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <io.h>
#include <sys/timeb.h>
# include <process.h>
#define SOCKET_CLOSE(s) do { closesocket((s)); (s) = INVALID_SOCKET; } while (0)
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_CLOSE(s) do { shutdown((s), SHUT_RDWR); close(s); } while (0)
#define SOCKET_ERROR -1
#endif