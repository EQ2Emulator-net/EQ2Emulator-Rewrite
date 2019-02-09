#include "stdafx.h"

#include "TCPServer.h"
#include "Stream.h"
#include "thread.h"
#include "../common/util.h"

TCPServer::TCPServer(bool bHostServer) : bHost(bHostServer) {
	FD_ZERO(&fds_master);
	bLooping = false;
	bReadyForSelect = true;
	bGotData = false;
}

TCPServer::~TCPServer() {
	for (auto& itr : Streams) {
		delete itr.second;
	}

	if (bLooping) {
		bLooping = false;
		select_thread.join();
	}
}

bool TCPServer::Open() {
	if (Sock != INVALID_SOCKET) {
		return true;
	}

	bLooping = true;
	bReadyForSelect = true;
	select_thread = ThreadManager::ThreadStart("TCPSelect", std::bind(&TCPServer::SelectThread, this));

	const char yes = 1;

	if ((Sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		return false;
	}

	sockaddr_in address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(Port);
	address.sin_addr.s_addr = Host;

	if (bHost) {
		setsockopt(Sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (::bind(Sock, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
			return false;
		}
	}
	else {
		if (connect(Sock, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
			return false;
		}

		Streams[Sock] = GetNewStream(address.sin_addr.s_addr, address.sin_port);
	}

	setsockopt(Sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int));
	setsockopt(Sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int));

	if (bHost) {
		if (listen(Sock, 10) == SOCKET_ERROR) {
			return false;
		}
	}

	return true;
}

void TCPServer::SelectThread() {
	timeval timeout;
	timeout.tv_usec = 0;

	while (bLooping) {
		if (!bReadyForSelect) {
			SleepMS(5);
			continue;
		}

		//Timeout of 5 seconds
		timeout.tv_sec = 5;

		fd_max = Sock;
		FD_SET(fd_max, &fds_master);
		fds_read = fds_master;

		int result = select(fd_max + 1, &fds_read, NULL, NULL, &timeout);
		if (result > 0) {
			bReadyForSelect = false;
			bGotData.store(true);
		}
	}
}

bool TCPServer::Process() {
	SOCKET i, fd_new;
	sockaddr_storage addr_s;
	socklen_t addr_len;
	unsigned char buf[4096];
	int count;

	if (bGotData.exchange(false)) {
		for (i = 0; i <= fd_max; i++) {
			if (FD_ISSET(i, &fds_read)) {
				if (i == fd_max && bHost) {
					addr_len = sizeof(addr_s);

					//We have a new client, so accept it.
					if ((fd_new = accept(fd_max, reinterpret_cast<sockaddr*>(&addr_s), &addr_len)) == INVALID_SOCKET) {
						//LogError(LOG_TCP, 0, "Unable to accept new TCP connection: %s", Socket::SocketError().c_str());
						bReadyForSelect = true;
						return false;
					}
					else {
						assert(addr_s.ss_family == AF_INET);
						const sockaddr_in& addr = *reinterpret_cast<sockaddr_in*>(&addr_s);

						//Add the client's socket to the fdset and update the max fd
						FD_SET(fd_new, &fds_master);
						if (fd_new > fd_max)
							fd_max = fd_new;

						Stream* client = GetNewStream(addr.sin_addr.s_addr, addr.sin_port);
						client->SetSocket(fd_new);

						//Add the client to our streams
						Streams[fd_new] = client;
					}
				}
				else {
					//We have data from an existing client. Determine if it's a disconnect or actual data.
					auto itr = Streams.find(i);
					assert(itr != Streams.end());

					Stream* client = itr->second;

					if ((count = recv(i, reinterpret_cast<char*>(buf), sizeof(buf), 0)) <= 0) {
						//Disconnect
						FD_CLR(i, &fds_master);
						Streams.erase(itr);

						//Do one last process on this client before deletion
						client->Process();
						delete client;
					}
					else {
						//We have data from the client.
						client->Process(buf, count);
					}
				}
			}
		}

		bReadyForSelect = true;
	}

	//Run any processing on these streams now
	for (auto& itr : Streams) {
		itr.second->Process();
	}

	return true;
}