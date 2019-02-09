#include "stdafx.h"

#include "Server.h"

unsigned int Server::InitializeCount = 0;

Server::Server() {
	Sock = INVALID_SOCKET;
	Port = 0;
	Host = 0;

	Server::InitializeCount++;
#ifdef _WIN32
	if (Server::InitializeCount == 1) {
		WORD version = MAKEWORD(1, 1);
		WSADATA wsadata;
		WSAStartup(version, &wsadata);
	}
#endif
}

Server::~Server() {
	Server::InitializeCount--;
#ifdef _WIN32
	if (Server::InitializeCount == 0)
		WSACleanup();
#endif

	if (Sock != INVALID_SOCKET) {
		closesocket(Sock);
	}
}

void Server::SetHost(const char* host) {
	Host = inet_addr(host);
}

void Server::SetPort(unsigned int port) {
	Port = port;
}