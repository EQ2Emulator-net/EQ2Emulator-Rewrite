#pragma once

#include "Server.h"
#include <map>
#include <string>
#include <thread>
#include "Mutex.h"

class Stream;

class UDPServer : public Server {
public:
	~UDPServer();
	bool Open() override;
	void StreamDisconnected(Stream* stream) override;

protected:
	UDPServer() : bLooping(false) { streamLock.SetName("UDPServer::streamLock"); }
	std::map<std::string, Stream*> Streams;
	Mutex streamLock;

private:
	std::thread read_thread;
	bool bLooping;

	void ReaderThread();
};