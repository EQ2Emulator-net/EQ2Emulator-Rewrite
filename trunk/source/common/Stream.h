#pragma once


class Stream {
public:
	Stream(unsigned int ip, unsigned short port);

	virtual void Process(unsigned char* data, unsigned int length);
	void SetLastPacketTime(unsigned int time) { LastPacketTime = time; }

protected:
	unsigned int LastPacketTime;
	unsigned int ReceivedPackets;

private:
	unsigned int RemoteIP;
	unsigned short RemotePort;

};