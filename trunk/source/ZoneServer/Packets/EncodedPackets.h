#pragma once

#include "../../common/EncodedBuffer.h"
#include <memory>
#include <array>
#include <map>

enum EEncodedPackets {
	EEncoded_UpdatePosition,
	EEncoded_UpdateCharSheet,
	EEncoded_UpdateSpawnVis,
	EEncoded_UpdateSpawnPos,
	EEncoded_UpdateSpawnInfo,
	EEncoded_COUNTVAR
};

class EncodedPackets {
public:
	EncodedPackets() {
		bufferLock.SetName("EncodedPackets::bufferLock");
	}

	~EncodedPackets() = default;

	std::shared_ptr<EncodedBuffer> GetBuffer(EEncodedPackets t, uint32_t ref = 0, bool* bNewOut = nullptr);
	void RemoveBuffer(EEncodedPackets t, uint32_t ref = 0);

private:
	std::array<std::map<uint32_t, std::shared_ptr<EncodedBuffer> >, EEncoded_COUNTVAR> buffers;
	Mutex bufferLock;

	void TypeRefCheck(EEncodedPackets t, uint32_t ref);
};