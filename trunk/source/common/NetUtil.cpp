#include "stdafx.h"

#include "NetUtil.h"

int32_t NetUtil::SocketErrno() {
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

std::string NetUtil::SocketError() {
	char errbuf[512];

#ifdef _WIN32
	if (strerror_s(errbuf, sizeof(errbuf), SocketErrno()) != 0) {
		return "appStrError : strerror_s unsuccessful!";
	}
#elif (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
	if (strerror_r(SocketErrno(), errbuf, sizeof(errbuf)) != 0) {
		return "appStrError : strerror_s unsuccessful!";
	}
#else
	return strerror_r(SocketErrno(), errbuf, sizeof(errbuf));
#endif

	return errbuf;
}