#pragma once

#include "Server.h"
#include <map>
#include <future>
#include <atomic>

class TCPServer : public Server {
public:
	~TCPServer();

	bool Open() override;
	bool Process() override;

protected:
	TCPServer(bool bHost);
private:
	bool bHost;
	bool bLooping;
	bool bReadyForSelect;
	std::atomic_bool bGotData;
	std::map<SOCKET, Stream*> Streams;
	fd_set fds_master;
	fd_set fds_read;
	SOCKET fd_max;
	std::thread select_thread;

	void SelectThread();
};