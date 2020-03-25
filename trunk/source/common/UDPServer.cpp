#include "stdafx.h"

#include "UDPServer.h"
#include "log.h"
#include "Stream.h"
#include "thread.h"
#include "NetUtil.h"

UDPServer::~UDPServer() {
	if (bLooping) {
		bLooping = false;
		read_thread.join();
	}

	/*for (auto& itr : Streams) {
		delete itr.second;
	}*/
}

bool UDPServer::Open() {
	if (Sock != INVALID_SOCKET) {
		return true;
	}

	bLooping = true;
	read_thread = ThreadManager::ThreadStart("UDPReader", std::bind(&UDPServer::ReaderThread, this));

	sockaddr_in address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(Port);
	address.sin_addr.s_addr = Host;//htonl(INADDR_ANY);

	Sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (Sock < 0) {
		LogError(LOG_NET, 0, "Failed to open socket. (%s)", NetUtil::SocketError().c_str());
		return false;
	}

	if (::bind(Sock, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0) {
		//_close(static_cast<int>(Sock));
		Sock = -1;
		LogError(LOG_NET, 0, "Failed to bind socket.");
		return false;
	}

#ifdef _WIN32
	unsigned long nonblock = 1;
	ioctlsocket(Sock, FIONBIO, &nonblock);
#else
	fcntl(Sock, F_SETFL, O_NONBLOCK);
#endif

	LogInfo(LOG_NET, 0, "Listening on %s:%u", inet_ntoa(address.sin_addr), Port);
	return true;
}

void UDPServer::ReaderThread() {
	fd_set readset;
	std::map<std::string, std::shared_ptr<Stream>>::iterator stream_itr;
	int num;
	int length;
	unsigned char buffer[2048];
	sockaddr_in from;
	timeval sleep_time;

	while (bLooping) {

		//Check if we need to remove any streams
		m_clientRemovals.Lock();
		if (!clientRemovals.empty()) {
			WriteLocker lock(streamLock);
			for (auto& itr : clientRemovals) {
				if ((stream_itr = Streams.find(itr)) != Streams.end()) {
					LogDebug(LOG_NET, 0, "Removing client.");
					//delete stream_itr->second;
					Streams.erase(stream_itr);
				}
				else {
					LogDebug(LOG_NET, 0, "Stream not found in disconnect.");
				}
			}
			clientRemovals.clear();
		}
		m_clientRemovals.Unlock();

		FD_ZERO(&readset);
		FD_SET(Sock, &readset);

		sleep_time.tv_sec = 30;
		sleep_time.tv_usec = 0;

		if ((num = select(static_cast<int>(Sock + 1), &readset, NULL, NULL, &sleep_time)) < 0) {
			LogError(LOG_NET, 0, "select error");
			continue;
		}
		else if (num == 0)
			continue;

		int socklen = sizeof(sockaddr_in);

		if (FD_ISSET(Sock, &readset)) {
#ifdef _WIN32
			if ((length = recvfrom(Sock, (char*)buffer, 2048, 0, (struct sockaddr*)&from, &socklen)) < 0)
#else
			if ((length = recvfrom(Sock, buffer, 2048, 0, (sockaddr*)&from, (socklen_t*)&socklen)) < 0)
#endif // _WIN32
			{
				/*LogError(LOG_NET, 0, "recvfrom error (%i)", WSAGetLastError());
				return false;*/
			}
			else {
				LogError(LOG_NET, 0, "received %i", length);
				char temp[25];
				sprintf(temp, "%u.%d", ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));
				LogError(LOG_NET, 0, "temp = %s", temp);
				WriteLocker lock(streamLock);
				if ((stream_itr = Streams.find(temp)) == Streams.end()) {
					LogError(LOG_NET, 0, "new stream");
					std::shared_ptr<Stream> s = GetNewStream(from.sin_addr.s_addr, from.sin_port);
					//Stream* s = GetNewStream(from.sin_addr.s_addr, from.sin_port);
					s->SetServer(this);
					//Streams[temp] = s;
					AddStream(s, temp);
					s->Process(buffer, length);
					//s->SetLastPacketTime();
				}
				else {
					LogError(LOG_NET, 0, "found stream");
					std::shared_ptr<Stream> currentStream = stream_itr->second;

					currentStream->Process(buffer, length);
					//currentStream->SetLastPacketTime();
				}
			}
		}
		else {
			LogError(LOG_NET, 0, "FD_ISSET failed");
		}
	}
}

void UDPServer::StreamDisconnected(std::shared_ptr<Stream> stream) {
	std::map<std::string, Stream*>::iterator stream_itr;
	SpinLocker lock(m_clientRemovals);
	clientRemovals.push_back(stream->ToString());
}

void UDPServer::AddStream(std::shared_ptr<Stream> stream, std::string key) {
	Streams[key] = stream;
}