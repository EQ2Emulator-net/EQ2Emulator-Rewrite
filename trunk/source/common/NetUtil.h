#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <sys/socket.h>
#endif

class NetUtil {
public:
	static int32_t SocketErrno();
	static std::string SocketError();
};