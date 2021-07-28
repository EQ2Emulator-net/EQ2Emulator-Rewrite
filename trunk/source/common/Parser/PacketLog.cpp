#include "stdafx.h"
#include "PacketLog.h"
#include <fstream>
#include "ParserDatabase.h"
#include "../../ZoneServer/Packets/OP_LoginByNumRequestMsg_Packet.h"
#include "../../WorldServer/Packets/OP_LoginRequestMsg_Packet.h"

PacketLog::PacketLog(const std::string& file) : filename(file), logVersion(0), log_id(0) {

}

inline bool IsHexCharacter(char c) {
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

size_t GetDataStart(const std::string& line) {
	size_t pos = 0;
	for (auto& itr : line) {
		if (!IsHexCharacter(itr)) {
			if (pos++ < 4) return std::string::npos;
			if (itr == '\t') break;
			if (itr != ':') return std::string::npos;
		}
		else ++pos;
	}
	return pos;
}

inline unsigned char GetHexCharValue(const char c) {
	if (c >= 'A') return c - ('A' - 10);
	return c - '0';
}

inline char HexTextToByte(const char* text) {
	union {
		unsigned char byte;
		char ret;
	};

	byte = GetHexCharValue(text[0]) * 16;
	byte += GetHexCharValue(text[1]);

	return ret;
}

bool PacketLog::TransformPackets() {
	std::ifstream f(filename);
	if (!f) {
		return false;
	}

	std::ostringstream currentPacket;
	bool bInPacket = false;
	bool bServerPacket = false;
	bool bLoginByNum = false;
	bool bLoginRequest = false;
	std::string line;
	std::string clientAddr;
	std::string serverAddr;

	uint32_t lineNum = 0;
	uint32_t packetLine = 0;

	while (std::getline(f, line)) {
		++lineNum;
		if (bInPacket) {
			if (line.empty()) {
				bInPacket = false;
				if (bLoginByNum) {
					bLoginByNum = false;
					std::string p = currentPacket.str();
					logVersion = ReadLoginByNumRequest(reinterpret_cast<const unsigned char*>(p.c_str()), static_cast<uint32_t>(p.size()));
				}
				else if (bLoginRequest) {
					bLoginRequest = false;
					std::string p = currentPacket.str();
					logVersion = ReadLoginRequest(reinterpret_cast<const unsigned char*>(p.c_str()), static_cast<uint32_t>(p.size()));
				}
				else {
					AddPacket(currentPacket, bServerPacket, packetLine);
				}
				currentPacket.str("");
				continue;
			}

			size_t dataPos = GetDataStart(line);

			if (dataPos == std::string::npos) {
				//We're on a header line
				continue;
			}

			size_t end = line.size();
			
			char bytes[16];
			size_t i = 0;
			//Each byte is 2 hex characters followed by a space, I could use a regex here but it's too damn slow
			for (; dataPos != end && i < 16; dataPos += 3, ++i) {
				const char* const text = line.c_str() + dataPos;
				if (!IsHexCharacter(*text)) {
					break;
				}
				bytes[i] = HexTextToByte(text);
			}

			currentPacket.write(bytes, i);
		}
		else if (line.find("--") == 0) {
			if (line.find("-- Server Network Status Update") == 0 || line.find("-- Client Network Status Update") == 0
				|| line.find("-- Session Response Packet") == 0 || line.find("-- Session Request Packet") == 0
				|| line.find("-- Close Connection Packet") == 0 || line.find("-- Server Keygen Request --") == 0)
			{
				continue;
			}

			bool bKeygenResponse = line.find("-- Client Keygen Response") == 0;
			bLoginByNum = !bKeygenResponse && line.find("-- OP_LoginByNumRequestMsg --") == 0;
			bLoginRequest = !bLoginByNum && !bKeygenResponse && line.find("-- OP_LoginRequestMsg --") == 0;

			if ((bLoginByNum || bLoginRequest) && logVersion != 0) {
				bLoginByNum = false;
				bLoginRequest = false;
				continue;
			}

			//Get the address line
			for (int i = 0; i < 2; i++) std::getline(f, line);

			packetLine = lineNum;
			lineNum += 2;
			
			if (bKeygenResponse) {
				clientAddr = line.substr(0, line.find_first_of(' '));
				serverAddr = line.substr(line.find_last_of(' ') + 1, std::string::npos);
				continue;
			}

			bServerPacket = strncmp(line.c_str(), clientAddr.c_str(), clientAddr.size()) != 0;
			bInPacket = true;
		}
	}

	opcodeLookup = std::move(ParserDatabase::GetGlobal()->LoadOpcodesForVersion(logVersion));

	if (opcodeLookup.empty()) {
		return false;
	}

	SortClientCommands();

	return true;
}

//NOTE: Need to filter out opcode 0/1 packets (OP_LoginRequest and OP_LoginByNumRequest) before this function based on the name in the log
//This is due to working around an error in packet collector that adds an extra 00 to the front of some packets
void PacketLog::AddPacket(const std::ostringstream& ss, bool bServerPacket, uint32_t lineNumber) {
	size_t start = 0;
	std::string bytes = ss.str();

	//Find the opcode

			//Collector bug? some packets have an extra 0 on the front? Skip the first 0 and hope for the best...
			//--OP_GuildBankEventListMsg--
			//	8 / 9 / 2020 16:26 : 42
			//	SERVER -> Client
			//	0000 : 00 01 FF 17 01 02 DA AA 26 5D 02 80 E4 0D 46 38 ........&]....F8
			//	0010:	8D E2 42 ED 5E 16 00 00 00 68 00 42 6F 6F 6A 69 ..B.^....h.Booji
			//	0020 : 6F 20 6C 6F 6F 74 65 64 20 74 68 65 20 46 61 62 o looted the Fab
			//	0030 : 6C 65 64 20 5C 61 49 54 45 4D 20 2D 31 38 39 33 led \aITEM - 1893
			//	0040:	35 35 38 36 36 33 20 32 31 31 30 36 36 31 36 31 558663 211066161
			//	0050 : 35 3A 56 65 69 6C 77 61 6C 6B 65 72 27 73 20 41 5 : Veilwalker's A
			//	0060 : 64 6F 72 6E 6D 65 6E 74 20 6F 66 20 49 6E 63 72 dornment of Incr
			//	0070 : 65 61 73 65 64 20 43 72 69 74 69 63 61 6C 73 5C eased Criticals\
			//	0080:	2F 61 2E / a.

			//--OP_DispatchClientCmdMsg--
			//	8 / 9 / 2020 16:26 : 42
			//	SERVER -> Client
			//	0000 : 00 00 3C 12 00 00 00 FF EA 02 00 0A 00 49 6E 71 .. < ..........Inq
			//	0010 : 75 69 73 69 74 6F 72 00 00                      uisitor..

			//--OP_Unknown509--
			//	8 / 9 / 2020 16:26 : 47
			//	Client -> SERVER
			//	0000 : 00 FF FD 01 02 00 00 00 39 71 AB C9 4E 60 1E 5B ........9q..N`.[

	if (bytes[0] == 0) {
		//Skip the SOE protocol op/sequence
		if (bytes[1] == 9) start = 4;
		else if (!bServerPacket) start = 1;
	}

	if (bServerPacket) {
		//Skip the compressed bool byte, check for the extra 00 mentioned above
		if (start++ == 0 && static_cast<unsigned char>(bytes[1]) <= 1) {
			++start;
		}
	}

	uint16_t opcode = static_cast<unsigned char>(bytes[start++]);
	if (opcode == 0xFF) {
		opcode = static_cast<unsigned char>(bytes[start++]);
		opcode += static_cast<unsigned char>(bytes[start++]) << 8;
	}

	bytes = bytes.substr(start, std::string::npos);

	packets[opcode].emplace_back(lineNumber, std::move(bytes));
}

uint32_t PacketLog::ReadLoginByNumRequest(const unsigned char* data, uint32_t size) {
	uint32_t offset = 0;
	if (data[0] == 0) {
		//Set the offset 1 byte past the opcode
		if (data[1] == 9) offset = 5;
		else offset = 2;
	}
	else offset = 1;

	uint32_t pver = OP_LoginByNumRequestMsg_Packet::DetermineStructVersion(data, offset, size);
	OP_LoginByNumRequestMsg_Packet p(pver);
	p.Read(data, offset, size);
	return static_cast<uint32_t>(p.version);
}

uint32_t PacketLog::ReadLoginRequest(const unsigned char* data, uint32_t size) {
	uint32_t offset = 1;
	if (data[0] == 0) {
		//Set the offset 1 byte past the opcode
		if (data[1] == 9) offset = 5;
		//Opcode 0 so to check for the "extra" 0 hope that the first element is set in the packet normally
		//else if (data[1] == 0) offset = 2;
	}

	uint32_t version;

	for (;;) {
		uint32_t pver = OP_LoginRequestMsg_Packet::DetermineStructVersion(data, size, offset);
		OP_LoginRequestMsg_Packet p(pver);
		if (!p.Read(data, offset, size) && offset == 1) offset = 2;
		else {
			version = static_cast<uint32_t>(p.Version);
			break;
		}
	}
	return version;
}