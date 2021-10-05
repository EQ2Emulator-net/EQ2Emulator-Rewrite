#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "OP_PlayFlythroughCmd_Packet.h"

class OP_ZoneInfoMsg_Packet : public EQ2Packet {
public:
	OP_ZoneInfoMsg_Packet(uint32_t version)
		: EQ2Packet(version), flythrough(version) {
		RegisterElements();

		//server1 = "";
		//server2 = "";
		unknown1[0] = 0;
		unknown1[1] = 0;
		account_flags_1 = 0;
		account_flags_2 = 0;
		unknown2b = 0;
		unknown3b = 0;
		unknown3c = 0;
		auction_port = 0;
		x = 0;
		y = 0;
		z = 0;
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		minute = 0;
		seconds = 0;
		unknown7[0] = 0;
		unknown7[1] = 0;
		unknown8ArraySize = 0;
		unknown9 = 0;
		zone_flags = 0;
		levelA = 64;
		levelB = 91;
		unknown10c = 0;
		permission_level = 0;		
		num_adv = 0;
		num_client_setup = 0;
		unknown11 = 0;
		tab_count = 0;
		unknown67650Count = 0;
		unk69239a = 0;

		unknown_mj = 0;
		unkArrayCount = 0;
		bHasFlythrough = false;
	}

	std::string server1;
	std::string server2;
	uint8_t unknown1[2];
	std::string account_flags_1_string;
	std::string account_flags_2_string;
	uint64_t account_flags_1;
	uint64_t account_flags_2;
	uint32_t unknown2b;
	uint32_t unknown3b;
	uint32_t unknown3c;
	std::string auction_website;
	uint32_t auction_port;
	std::string upload_page;
	std::string upload_key;
	std::string upload_key2;
	std::string zone;
	std::string zone2;
	std::string parent_zone;
	std::string zone_unknown2;
	std::string zone_desc;
	std::string char_name;
	std::string motd;
	float x;
	float y;
	float z;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t seconds;
	float unknown7[2];
	uint8_t unknown8ArraySize;
	float unknown9;
	uint32_t zone_flags;
	//these 2 appear to be levels (I see the max combat level of 120 being sent alot) but not sure what they are exactly
	uint8_t levelA;
	uint8_t levelB;
	int32_t unknown10c;
	uint8_t permission_level;
	bool bHasFlythrough;
	int32_t unk69239a;
	Substruct_FlythroughDesc flythrough;

	uint32_t num_adv;
	struct AdvArray : public PacketSubstruct {
		std::string adv_name;
		uint16_t adv_id;

		AdvArray(uint32_t ver = 0) : PacketSubstruct(ver, true) {
			//adv_name = "";
			adv_id = 0;

			RegisterElements();
		}

		void RegisterElements() {
			Register16String(adv_name);
			RegisterUInt16(adv_id);
		}
	};
	std::vector<AdvArray> adv_array;

	uint32_t num_client_setup;
	struct ClientCommands : public PacketSubstruct {
		std::string client_cmd;

		ClientCommands(uint32_t ver = 0) : PacketSubstruct(ver, true) {
			//client_cmds = "";

			RegisterElements();
		}

		void RegisterElements() {
			Register8String(client_cmd);
		}
	};
	std::vector<ClientCommands> client_cmd_array;

	int32_t unknown11;

	uint16_t num_news_items;
	struct NewsItem : public PacketSubstruct {
		uint32_t news_id;
		std::string news;

		NewsItem(uint32_t ver = 0) : PacketSubstruct(ver, true) {
			news_id = 0;
			//news = "";

			RegisterElements();
		}

		void RegisterElements() {
			RegisterUInt32(news_id);
			Register16String(news);
		}
	};
	std::vector<NewsItem> news_item_array;

	uint32_t tab_count;
	struct Tabs : public PacketSubstruct {
		uint32_t tab_index;
		std::string tab_name;

		Tabs(uint32_t ver = 0) : PacketSubstruct(ver, true) {
			tab_index = 0;
			//tab_name = "";

			RegisterElements();
		}

		void RegisterElements() {
			RegisterUInt32(tab_index);
			Register16String(tab_name);
		}
	};
	std::vector<Tabs> tab_array;
	
	uint32_t unknown67650Count;
	uint8_t unknown_mj;
	uint32_t unkArrayCount;

	struct UnknownArray : public PacketSubstruct {
		int32_t unkArr1;
		int32_t unkArr2;
		int32_t unkArr3;
		int32_t unkArr4;
		int32_t unkArr5;

		UnknownArray(uint32_t ver = 0) : PacketSubstruct(ver, true),
			unkArr1(0), unkArr2(0), unkArr3(0), unkArr4(0), unkArr5(0) {
			RegisterElements();
		}

		void RegisterElements() {
			RegisterInt32(unkArr1);
			RegisterInt32(unkArr2);
			RegisterInt32(unkArr3);
			RegisterInt32(unkArr4);
			RegisterInt32(unkArr5);
		}
	};

	std::vector<UnknownArray> unkArray;

	void PreWrite() override {

	}

private:
	void RegisterElements() {
		const uint32_t version = GetVersion();
		if (version > 283) {
			Register8String(server1);
			Register8String(server2);
			uint8_t& Unknown1 = unknown1[0];
			RegisterUInt8(Unknown1)->SetCount(2);
			if (version >= 63182) {
				std::string& account_flags_1 = account_flags_1_string;
				std::string& account_flags_2 = account_flags_2_string;
				Register32String(account_flags_1);
				Register32String(account_flags_2);
			}
			else {
				if (version < 1142) {
					RescopeToReference(account_flags_1, uint32_t);
					RescopeToReference(account_flags_2, uint32_t);
					RegisterUInt32(account_flags_1);
					RegisterUInt32(account_flags_2);
				}
				else {
					RegisterUInt64(account_flags_1);
					RegisterUInt64(account_flags_2);
				}

				Register8String(auction_website);
				RegisterUInt32(auction_port);
				Register8String(upload_page);
			}
			Register8String(upload_key);
			if (version >= 1067)
				Register8String(upload_key2);
		}
		Register8String(zone);
		Register8String(zone2);
		Register8String(parent_zone);
		Register8String(zone_unknown2);
		Register8String(zone_desc);
		Register8String(char_name);
		if (version >= 957 && version < 959)
			Register16String(motd);
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
		RegisterUInt16(year);
		RegisterUInt8(month);
		RegisterUInt8(day);
		RegisterUInt8(hour);
		RegisterUInt8(minute);
		RegisterUInt8(seconds);
		if (version > 283) {
			float& Unknown7 = unknown7[0];
			RegisterFloat(Unknown7)->SetCount(2);
			RegisterUInt8(unknown8ArraySize);
			auto e = RegisterBool(bHasFlythrough);
			RegisterSubstruct(flythrough)->SetIsVariableSet(e);
			RegisterFloat(unknown9);
		}
		if (version >= 942) {
			RegisterUInt8(levelA);
			RegisterUInt8(levelB);
		}

		RegisterUInt32(zone_flags);
		if (version < 284) {
			return;
		}
		if (version >= 1142) {
			RegisterInt32(unknown10c);
			if (version >= 69239) {
				RegisterInt32(unk69239a);
			}
			RegisterUInt8(permission_level);
		}
		
		PacketUInt32* asize32 = RegisterUInt32(num_adv);
		asize32->SetMyArray(RegisterArray(adv_array, AdvArray));

		asize32 = RegisterUInt32(num_client_setup);
		asize32->SetMyArray(RegisterArray(client_cmd_array, ClientCommands));

		RegisterInt32(unknown11);

		if (version >= 959 && version < 1096) {
			PacketUInt16* asize16 = RegisterUInt16(num_news_items);
			asize16->SetMyArray(RegisterArray(news_item_array, NewsItem));
		}

		if (version >= 67650) {
			RegisterUInt32(unknown67650Count);
		}
		else if (version >= 1193) {
			asize32 = RegisterUInt32(tab_count);
			asize32->SetMyArray(RegisterArray(tab_array, Tabs));
		}

		if (version >= 63319) {
			RegisterUInt8(unknown_mj);
			asize32 = RegisterUInt32(unkArrayCount);
			asize32->SetMyArray(RegisterArray(unkArray, UnknownArray));
		}
	}

};