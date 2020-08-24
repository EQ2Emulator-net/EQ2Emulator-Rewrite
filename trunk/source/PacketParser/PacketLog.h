#pragma once

#include <cstdint>
#include <string>

class PacketLog {
public:
	PacketLog(const std::string& file);
	~PacketLog() = default;

	bool TransformPackets();

	std::string filename;
};