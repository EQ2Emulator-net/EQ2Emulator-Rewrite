#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include <string>
#include <vector>

class Substruct_SpawnHeader : public PacketSubstruct {
public:
	~Substruct_SpawnHeader() = default;

	uint32_t spawn_id;
	uint32_t spawn_anim;
	uint32_t unknown2;
	uint32_t crc;
	std::string default_command;
	float max_distance;
	uint32_t time_stamp;
	uint16_t index;
	uint8_t command_list_size;
	uint8_t group_size;

	struct Substruct_Group : public PacketSubstruct {
		uint32_t group_spawn_id;

		Substruct_Group() : PacketSubstruct(0) {
			
		}

		void RegisterElements() override {
			RegisterUInt32(group_spawn_id);
		}
	};

	std::vector<Substruct_Group> group_array;

	struct Substruct_CommandList : public PacketSubstruct {
		std::string command_list_name;
		float command_list_max_distance;
		uint8_t command_list_error_code;
		uint8_t command_list_unknown;
		std::string command_list_command;

		Substruct_CommandList() : PacketSubstruct(0), command_list_max_distance(0.f), command_list_error_code(0), command_list_unknown(0) {
			
		}

		void RegisterElements() override {
			Register16String(command_list_name);
			RegisterFloat(command_list_max_distance);
			auto e = RegisterUInt8(command_list_error_code);
			RegisterUInt8(command_list_unknown);
			Register16String(command_list_command)->SetIsVariableSet(e);
		}
	};

	std::vector<Substruct_CommandList> command_list_array;

	Substruct_SpawnHeader(uint32_t version) : PacketSubstruct(version) {
		spawn_anim = 0;
		group_size = 0;
		max_distance = 0;
		time_stamp = 0;
		crc = 0;
		index = 0;
		command_list_size = 0;
		spawn_id = 0;
		unknown2 = 0;
	}

	void RegisterElements() {
		RegisterOversizedByte(index);
		RegisterUInt32(spawn_id);

		if (version >= 57080) {
			RegisterUInt32(spawn_anim);
		}
		else if (version > 283) {
			uint16_t& spawn_anim = reinterpret_cast<uint16_t&>(this->spawn_anim);
			RegisterUInt16(spawn_anim);
		}

		if (version > 283) {
			RegisterUInt32(unknown2);
			RegisterUInt32(crc);
		}
		PacketUInt8* clsize = RegisterUInt8(command_list_size);
		clsize->SetMyArray(RegisterArray(command_list_array, Substruct_CommandList));
		Register16String(default_command);
		RegisterFloat(max_distance);
		PacketUInt8* gsize = RegisterUInt8(group_size);
		gsize->SetMyArray(RegisterArray(group_array, Substruct_Group));
		RegisterUInt32(time_stamp);
	}
};