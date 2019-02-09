#pragma once

#include "Server.h"
#include <map>
#include <string>

class Stream;

class UDPServer : public Server {
public:
	~UDPServer();
	bool Open() override;
	bool Process() override;
	void StreamDisconnected(Stream* stream) override;

protected:
	UDPServer() = default;
	std::map<std::string, Stream*> Streams;
};