#pragma once

#include "Server.h"
#include <map>
#include <future>
#include <atomic>
#include "Mutex.h"

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
	std::map<SOCKET, Stream*> Streams;
	Mutex streamLock;
	std::thread read_thread;

	void ReaderThread();
};