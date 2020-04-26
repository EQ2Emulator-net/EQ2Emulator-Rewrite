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
	virtual void AddStream(std::shared_ptr<Stream> stream, uint64_t key);

protected:
	UDPServer() : bLooping(false) { streamLock.SetName("UDPServer::streamLock"); }
	std::map<uint64_t, std::shared_ptr<Stream> > Streams;
	Mutex streamLock;

private:
	std::vector<std::shared_ptr<Stream> > clientRemovals;
	SpinLock m_clientRemovals;

	std::thread read_thread;
	bool bLooping;

	void ReaderThread();
};