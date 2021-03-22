#include "stdafx.h"

#include "UDPServer.h"
#include "log.h"
#include "Stream.h"
#include "thread.h"
#include "NetUtil.h"
#include "timer.h"
#include "Packets/Packet.h"

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
	address.sin_addr.s_addr = htonl(INADDR_ANY);

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
	int num;
	int length;
	unsigned char buffer[2048];
	sockaddr_in from;
	timeval sleep_time;
	std::map<uint64_t, std::shared_ptr<Stream> >::iterator stream_itr;

	//30 seconds heartbeat timeout
	const uint32_t TIMEOUT_MS = 30000;

	sleep_time.tv_sec = 5;
	sleep_time.tv_usec = 0;

	while (bLooping) {
		uint32_t currentTime = Timer::GetServerTime();

		//Check for timeouts
		{
			ReadLocker lock(streamLock);
			for (auto& itr : Streams) {
				auto& stream = itr.second;
				if (stream->CheckHeartbeatDelta(currentTime) >= TIMEOUT_MS) {
					LogDebug(LOG_NET, 0, "Client timeout : %s", stream->ToString().c_str());
					SpinLocker l(m_clientRemovals);
					clientRemovals.push_back(stream);
					stream->ConnectionTimeout();
				}
			}
		}

		//Check if we need to remove any streams
		m_clientRemovals.Lock();
		if (!clientRemovals.empty()) {
			WriteLocker lock(streamLock);
			for (auto& itr : clientRemovals) {
				if ((stream_itr = Streams.find(itr->GetConnectionKey())) != Streams.end()) {
					LogDebug(LOG_NET, 0, "Removing client.");
					StreamDisconnected(itr);
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

		if ((num = select(static_cast<int>(Sock + 1), &readset, nullptr, nullptr, &sleep_time)) < 0) {
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
#endif
			{
				LogError(LOG_NET, 0, "recvfrom error (%s)", NetUtil::SocketError().c_str());
			}
			else {
				if (NetDebugEnabled()) {
					DumpBytes(buffer, length, "recvfrom", true);
				}
				uint64_t clientKey = static_cast<uint32_t>(from.sin_addr.s_addr);
				clientKey <<= 16;
				clientKey |= static_cast<uint16_t>(from.sin_port);
				WriteLocker lock(streamLock);
				if ((stream_itr = Streams.find(clientKey)) == Streams.end()) {
					if (NetDebugEnabled()) {
						LogError(LOG_NET, 0, "new stream");
					}
					std::shared_ptr<Stream> s = GetNewStream(from.sin_addr.s_addr, from.sin_port);
					s->SetServer(this);
					s->SetSocket(Sock);
					AddStream(s, clientKey);
					s->Process(buffer, length);
					s->UpdateHeartbeat(currentTime);
				}
				else {
					std::shared_ptr<Stream> currentStream = stream_itr->second;

					currentStream->Process(buffer, length);
					currentStream->UpdateHeartbeat(currentTime);
					if (currentStream->RequestNewClient()) {
						StreamDisconnected(currentStream);

						//We received a new session request for an old client, generate a new client for this key to reset our client state
						std::shared_ptr<Stream> s = GetNewStream(from.sin_addr.s_addr, from.sin_port);
						s->SetServer(this);
						s->SetSocket(Sock);
						AddStream(s, clientKey);
						s->Process(buffer, length);
						s->UpdateHeartbeat(currentTime);
					}
				}
			}
		}
		else {
			LogError(LOG_NET, 0, "FD_ISSET failed");
		}
	}
}

void UDPServer::AddStream(std::shared_ptr<Stream> stream, uint64_t key) {
	Streams[key] = stream;
}