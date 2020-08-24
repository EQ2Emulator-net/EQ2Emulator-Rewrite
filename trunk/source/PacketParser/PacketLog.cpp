#include "stdafx.h"
#include "PacketLog.h"
#include <fstream>
#include <array>

PacketLog::PacketLog(const std::string& file) : filename(file) {

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

	std::vector<std::ostringstream> packetBytes;

	std::ostringstream currentPacket;
	bool bInPacket = false;
	std::string line;

	while (std::getline(f, line)) {
		if (bInPacket) {
			if (line.empty()) {
				bInPacket = false;
				packetBytes.emplace_back(std::move(currentPacket));
				continue;
			}

			size_t dataPos = GetDataStart(line);

			if (dataPos == std::string::npos) {
				//We're on a header line
				continue;
			}

			size_t end = line.size();
			
			std::array<char, 16> bytes;
			size_t i = 0;
			//Each byte is 2 hex characters followed by a space, I could use a regex here but it's too damn slow
			for (; dataPos != end && i < 16; dataPos += 3, ++i) {
				const char* const text = line.c_str() + dataPos;
				if (!IsHexCharacter(*text)) {
					break;
				}
				bytes[i] = HexTextToByte(text);
			}

			currentPacket.write(bytes.data(), i);
		}
		else if (line.find("--") == 0) {
			bInPacket = true;
		}
	}

	//Make sure we aren't losing a packet on the end
	if (bInPacket) {
		packetBytes.emplace_back(std::move(currentPacket));
	}

	return true;
}