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
		expansions_enabled = 0;
		unknown3[0] = 0;
		unknown3[1] = 0;
		unknown3[2] = 0;
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
		unknown10b = 0;
		unknown10c = 0;

		unknown10d = 0;
		unknown10e = 0;
		permission_level = 0;
		unknown10f = 0;
		
		num_adv = 0;
		// Error parsing the struct, XMLStructConverter.Element is an unknown type
		num_client_setup = 0;
		// Error parsing the struct, XMLStructConverter.Element is an unknown type
		unknown11 = 0;
		tab_count = 0;
		// Error parsing the struct, XMLStructConverter.Element is an unknown type

		unknown_mj = 0;
		unknown_mj1 = 0;
		unknown_mj2 = 0;
		unknown_mj3 = 0;
		unknown_mj4 = 0;
		unknown_mj5 = 0;
		unknown_mj6 = 0;
		unknown_mj7 = 0;
		unknown_mj8 = 0;
		unknown_mj9 = 0;
		unknown_mj10 = 0;
		unknown_mj11 = 0;
		unknown_mj12 = 0;
		unknown_mj13 = 0;
		unknown_mj14 = 0;
		unknown_mj15 = 0;
		unknown_mj16 = 0;
		unknown_mj17 = 0;
		unknown_mj18 = 0;
		unknown_mj19 = 0;
		unknown_mj20 = 0;
		unknown_mj21 = 0;
		unknown_mj22 = 0;
		bHasFlythrough = false;
	}

	std::string server1;
	std::string server2;
	uint8_t unknown1[2];
	std::string bolUnknown4;
	std::string bolUnknown7;
	uint32_t expansions_enabled;
	uint32_t unknown3[3];
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
	uint16_t unknown10b;
	uint8_t unknown10c;
	uint8_t unknown10d;
	uint8_t unknown10e;
	uint8_t permission_level;
	uint8_t unknown10f;
	bool bHasFlythrough;
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
		std::string client_cmds;

		ClientCommands(uint32_t ver = 0) : PacketSubstruct(ver, true) {
			//client_cmds = "";

			RegisterElements();
		}

		void RegisterElements() {
			Register8String(client_cmds);
		}
	};
	std::vector<ClientCommands> client_cmd_array;

	uint32_t unknown11;

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
	
	uint8_t unknown_mj;
	uint32_t unknown_mj1;
	uint32_t unknown_mj2;
	uint32_t unknown_mj3;
	uint32_t unknown_mj4;
	uint32_t unknown_mj5;
	uint32_t unknown_mj6;
	uint32_t unknown_mj7;
	uint32_t unknown_mj8;
	uint32_t unknown_mj9;
	uint32_t unknown_mj10;
	uint32_t unknown_mj11;
	uint32_t unknown_mj12;
	uint32_t unknown_mj13;
	uint32_t unknown_mj14;
	uint32_t unknown_mj15;
	uint32_t unknown_mj16;
	uint32_t unknown_mj17;
	uint32_t unknown_mj18;
	uint32_t unknown_mj19;
	uint32_t unknown_mj20;
	uint32_t unknown_mj21;
	uint32_t unknown_mj22;

private:
	void RegisterElements() {
		if (GetVersion() > 283) {
			Register8String(server1);
			Register8String(server2);
			uint8_t& Unknown1 = unknown1[0];
			RegisterUInt8(Unknown1)->SetCount(2);
			if (GetVersion() >= 67650) {
				Register32String(bolUnknown4);
				Register32String(bolUnknown7);
			}
			else {
				RegisterUInt32(expansions_enabled);

				uint32_t& Unknown3 = unknown3[0];
				if (GetVersion() < 1142)
					RegisterUInt32(Unknown3);
				else
					RegisterUInt32(Unknown3)->SetCount(3);

				if (GetVersion() >= 63182) {
					RegisterUInt32(unknown2b);
					RegisterUInt32(unknown3b);
					RegisterUInt32(unknown3c);
				}
				Register8String(auction_website);
				RegisterUInt32(auction_port);
				Register8String(upload_page);
			}
			Register8String(upload_key);
			if (GetVersion() >= 1067)
				Register8String(upload_key2);
		}
		Register8String(zone);
		Register8String(zone2);
		Register8String(parent_zone);
		Register8String(zone_unknown2);
		Register8String(zone_desc);
		Register8String(char_name);
		if (GetVersion() >= 957 && GetVersion() < 959)
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
		if (GetVersion() > 283) {
			float& Unknown7 = unknown7[0];
			RegisterFloat(Unknown7)->SetCount(2);
			RegisterUInt8(unknown8ArraySize);
			auto e = RegisterBool(bHasFlythrough);
			RegisterSubstruct(flythrough)->SetIsVariableSet(e);
			RegisterFloat(unknown9);
		}
		RegisterUInt32(zone_flags);
		if (GetVersion() < 284) {
			return;
		}
		if (GetVersion() >= 942)
			RegisterUInt16(unknown10b);
		if (GetVersion() >= 1142) {
			RegisterUInt8(unknown10c);
			RegisterUInt8(unknown10d);
			RegisterUInt8(unknown10e);
			RegisterUInt8(permission_level);
			RegisterUInt8(unknown10f);
		}
		
		PacketUInt32* asize32 = RegisterUInt32(num_adv);
		asize32->SetMyArray(RegisterArray(adv_array, AdvArray));

		asize32 = RegisterUInt32(num_client_setup);
		asize32->SetMyArray(RegisterArray(client_cmd_array, ClientCommands));

		RegisterUInt32(unknown11);

		if (GetVersion() >= 959 && GetVersion() < 1096) {
			PacketUInt16* asize16 = RegisterUInt16(num_news_items);
			asize16->SetMyArray(RegisterArray(news_item_array, NewsItem));
		}

		if (GetVersion() >= 1193 && GetVersion() < 67696) {
			asize32 = RegisterUInt32(tab_count);
			asize32->SetMyArray(RegisterArray(tab_array, Tabs));
		}

		if (GetVersion() >= 63319) {
			RegisterUInt8(unknown_mj);
			RegisterUInt32(unknown_mj1);
			RegisterUInt32(unknown_mj2);
			RegisterUInt32(unknown_mj3);
			RegisterUInt32(unknown_mj4);
			RegisterUInt32(unknown_mj5);
			RegisterUInt32(unknown_mj6);
			RegisterUInt32(unknown_mj7);
			RegisterUInt32(unknown_mj8);
			RegisterUInt32(unknown_mj9);
			RegisterUInt32(unknown_mj10);
			RegisterUInt32(unknown_mj11);
			RegisterUInt32(unknown_mj12);
			RegisterUInt32(unknown_mj13);
			RegisterUInt32(unknown_mj14);
			RegisterUInt32(unknown_mj15);
			RegisterUInt32(unknown_mj16);
			RegisterUInt32(unknown_mj17);
			RegisterUInt32(unknown_mj18);
			RegisterUInt32(unknown_mj19);
			RegisterUInt32(unknown_mj20);
			RegisterUInt32(unknown_mj21);
			RegisterUInt32(unknown_mj22);
		}
	}

};