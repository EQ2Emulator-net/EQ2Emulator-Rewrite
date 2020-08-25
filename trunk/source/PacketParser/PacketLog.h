#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <sstream>

class PacketLog {
public:
	PacketLog(const std::string& file);
	~PacketLog() = default;

	bool TransformPackets();

	std::string filename;

	std::map<uint16_t, std::vector<std::string> > packets;

private:
	void AddPacket(const std::ostringstream& ss, bool bServerPacket);
};