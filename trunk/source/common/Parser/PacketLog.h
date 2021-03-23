#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

class PacketLog {
public:
	PacketLog(const std::string& file);
	~PacketLog() = default;

	bool TransformPackets();

	std::string filename;
	uint32_t logVersion;
	//<opcode, vector<pair<lineNumber, data> > >
	std::unordered_map<uint16_t, std::vector<std::pair<uint32_t, std::string> > > packets;
	std::unordered_map<std::string, uint16_t> opcodeLookup;

private:
	void AddPacket(const std::ostringstream& ss, bool bServerPacket, uint32_t lineNumber);
	uint32_t ReadLoginByNumRequest(const unsigned char* data, uint32_t size);
	uint32_t ReadLoginRequest(const unsigned char* data, uint32_t size);
#ifdef EQ2_ZONE
	//We're only parsing client commands from the zoneserver binary
	void SortClientCommands();
#else
	void SortClientCommands() {};
#endif
};