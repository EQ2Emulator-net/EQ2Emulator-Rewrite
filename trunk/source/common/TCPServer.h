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
	bool Close();

protected:
	TCPServer(bool bHost);

	std::map<SOCKET, Stream*> Streams;
	Mutex streamLock;
private:
	bool bHost;
	bool bLooping;
	std::thread read_thread;

	void ReaderThread();
};