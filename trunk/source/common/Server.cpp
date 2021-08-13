#include "stdafx.h"

#include "Server.h"
#include "NetUtil.h"
#include "log.h"

Server::Server() {
	Sock = INVALID_SOCKET;
	Port = 0;
	Host = 0;

#ifdef _WIN32
	class WSAInitializer {
		WSADATA wsadata;

	public:
		WSAInitializer() {
			WORD version = MAKEWORD(2, 2);
			if (int32_t err = WSAStartup(version, &wsadata)) {
				LogError(LOG_NET, 0, "Error initializing sockets with WSAStartup! (%s)", NetUtil::SocketError(err).c_str());
			}
		}

		~WSAInitializer() {
			WSACleanup();
		}
	};

	static WSAInitializer g_WSAInitializer;
#endif
}

Server::~Server() {
	if (Sock != INVALID_SOCKET) {
		SOCKET_CLOSE(Sock);
	}
}

void Server::SetHost(const char* host) {
	Host = inet_addr(host);
}

void Server::SetPort(unsigned int port) {
	Port = port;
}

uint32_t Server::GetPort() {
	return Port;
}

std::string Server::GetHostString() {
	char tmp[INET6_ADDRSTRLEN];

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = Host;
	sa.sin_port = 0;
	return inet_ntop(AF_INET, &sa.sin_addr, tmp, sizeof(tmp));
}