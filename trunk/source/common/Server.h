#pragma once

#include <map>
#include <string>
#include "Stream.h"

class Server {
public:

	Server();
	~Server();

	bool Open(bool tcp = false);

	bool Process();
	int GetSocket() { return Sock; }

	void StreamDisconnected(Stream* stream);

	void SetHost(const char* host);
	void SetPort(unsigned int port);
	
protected:
	virtual Stream* GetNewStream(unsigned int ip, unsigned short port);

	std::map<std::string, Stream*> Streams;

private:
	int Sock;
	unsigned int Port;
	unsigned int Host;
	static unsigned int InitializeCount;

};