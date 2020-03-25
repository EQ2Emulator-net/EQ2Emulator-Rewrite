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
	void StreamDisconnected(std::shared_ptr<Stream> stream) override;
	virtual void AddStream(std::shared_ptr<Stream> stream, std::string key);

protected:
	UDPServer() : bLooping(false) { streamLock.SetName("UDPServer::streamLock"); }
	std::map<std::string, std::shared_ptr<Stream> > Streams;
	Mutex streamLock;

private:
	std::vector<std::string> clientRemovals;
	SpinLock m_clientRemovals;

	std::thread read_thread;
	bool bLooping;

	void ReaderThread();
};