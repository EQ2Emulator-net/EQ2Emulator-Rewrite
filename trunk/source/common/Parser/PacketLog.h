#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <type_traits>
#include <memory>
#include "../util.h"

#ifdef EQ2_ZONE
#include "../../ZoneServer/Packets/OP_ClientCmdMsg_Packet.h"
#endif

class PacketLog {
public:
	PacketLog(const std::string& file);
	~PacketLog() = default;

	template<typename T, typename SubT = T>
	std::vector<std::pair<uint32_t, std::unique_ptr<SubT> > > FindPackets() {
		uint16_t op;
		static_assert(std::is_base_of_v<class EQ2Packet, T>);
		{
			T p(logVersion);
			p.FindOpcode();
#ifdef EQ2_ZONE
			if (std::is_base_of_v<OP_ClientCmdMsg_Packet, T>) 
				op = reinterpret_cast<OP_ClientCmdMsg_Packet&>(p).GetSubOpcode();
			else
#endif
				op = p.GetOpcode();
		}

		std::vector<std::pair<uint32_t, std::unique_ptr<SubT> > > ret;
		ret.reserve(packets[op].size());

		for (auto& itr : packets[op]) {
			uint32_t line = itr.first;
			const std::string& data = itr.second;
			std::unique_ptr<EQ2Packet> p(new T(logVersion));
			auto bytes = reinterpret_cast<const unsigned char*>(data.c_str());
			if (!p->Read(bytes, 0, static_cast<uint32_t>(data.size()))) {
				DumpBytes(bytes, static_cast<uint32_t>(data.size()), "Packet read out of bounds");
			}
			//Check for sub packets (specialized item packets for example)
			while (EQ2Packet* tmp = p->GetSubPacket()) {
				p.reset(tmp);
				if (!tmp->Read(bytes, 0, static_cast<uint32_t>(data.size()))) {
					DumpBytes(bytes, static_cast<uint32_t>(data.size()), "Packet read out of bounds");				
					break;
				}
			}

			SubT* subp = dynamic_cast<SubT*>(p.get());
			if (subp) {
				p.release();
				ret.emplace_back(std::make_pair(line, subp));
			}
		}

		return ret;
	}

	bool TransformPackets(bool bStopAtVersion = false);

	std::string filename;
	uint32_t logVersion;
	uint32_t log_id;
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