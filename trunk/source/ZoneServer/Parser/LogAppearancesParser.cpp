#include "stdafx.h"

#include "LogAppearancesParser.h"
#include "../Packets/OP_DressingRoom_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../Packets/OP_RemoteCmdMsg_Packet.h"
#include "../Packets/OP_DisplayTextCmd_Packet.h"
#include "../../common/Separator.h"

LogAppearancesParser::LogAppearancesParser(PacketLog& log, ParserDatabase& db) : LogParser(log, db) {
	std::map<uint32_t, std::string> requests;
	{
		//Find the try_on command index
		uint32_t cmd_index = 0xFFFFFFFF;
		{
			auto command_list = log.FindPackets<OP_SetRemoteCmdsMsg_Packet>();
			if (command_list.empty()) {
				return;
			}

			uint32_t tmp = 0;
			for (auto& cmd : command_list[0].second->commands_array) {
				if (cmd.name == "try_on") {
					cmd_index = tmp;
					break;
				}
				tmp++;
			}

			if (cmd_index == 0xFFFFFFFF) return;
		}

		//Now build a reference of packet lines for try_on requests
		auto packets = log.FindPackets<OP_RemoteCmdMsg_Packet>();
		for (auto& itr : packets) {
			auto& p = itr.second;
			uint32_t line = itr.first;
			if (p->command_handler == cmd_index) {
				requests[line] = std::move(itr.second->arguments);
			}
		}

		if (requests.empty()) return;
	}

	auto FindRequestForPacket = [&requests](uint32_t line) -> std::string {
		const std::string* req = nullptr;
		for (auto& itr : requests) {
			if (itr.first >= line) break;
			req = &itr.second;
		}

		if (req) return *req;
		return std::string();
	};

	auto packets = log.FindPackets<OP_DressingRoom_Packet>();
	if (packets.empty()) return;

	//auto errors = log.FindPackets<OP_DisplayTextCmd_Packet>();

	for (auto& itr : packets) {
		uint32_t line = itr.first;
		std::unique_ptr<OP_DressingRoom_Packet>& p = itr.second;

		std::string args = FindRequestForPacket(line);
		if (args.empty()) {
			LogError(LOG_PARSER, 0, "Could not find request for a dressing room packet on line %u!", line);
			continue;
		}

		Separator sep(args);

		if (strcmp(sep.GetString(0), "crc") != 0) {
			LogError(LOG_PARSER, 0, "Got a try_on command using type %s!", sep.GetString(0));
			continue;
		}

		if (strcmp(sep.GetString(1), "drss") != 0) {
			LogError(LOG_PARSER, 0, "Got a try_on command using sub-type %s!", sep.GetString(1));
			continue;
		}

		if (dynamic_cast<DressingRoom_NetAppearance_Packet*>(p.get())) {
			LogDebug(LOG_PARSER, 0, "Got a net appearance dressing room packet (%s).", args.c_str());
			continue;
		}

		auto* dr = static_cast<DressingRoom_Standard_Packet*>(p.get());

		DatabaseRow row;
		row.m_tableName = "raw_appearances";

		row.RegisterField("log_id", log.log_id);
		row.RegisterField("line", line);
		row.RegisterField("soe_item_id_unsigned", static_cast<uint32_t>(sep.GetInt(2)));
		row.RegisterField("soe_item_crc_unsigned", static_cast<uint32_t>(sep.GetInt(3)));
		row.RegisterField("equip_type", dr->appearance_type);
		EQ2ColorFloat& color = dr->color;
		row.RegisterField("red", static_cast<int32_t>(color.Red * 255.f));
		row.RegisterField("green", static_cast<int32_t>(color.Green * 255.f));
		row.RegisterField("blue", static_cast<int32_t>(color.Blue * 255.f));
		EQ2ColorFloat& h = dr->highlight;
		row.RegisterField("highlight_red", static_cast<int32_t>(h.Red * 255.f));
		row.RegisterField("highlight_green", static_cast<int32_t>(h.Green * 255.f));
		row.RegisterField("highlight_blue", static_cast<int32_t>(h.Blue * 255.f));
		row.RegisterField("vis_state", dr->vis_state);
		row.RegisterField("vis_state2", dr->vis_state2);
		row.RegisterField("reforging_deco", dr->reforging_decoration);
		row.RegisterField("mount_type", dr->mount_type);
		row.RegisterField("slot", dr->slot);
		//Confirmed those 2 are item id/crc but not always sent
		//row.RegisterField("unk_id1", dr->item_id);
		row.RegisterField("unk_id2", dr->item_id2);
		//row.RegisterField("unk_crc", dr->item_crc);
		row.RegisterField("bEquipment", dr->bEquipment);
		row.RegisterField("b1hWeapon", dr->b1hWeapon);
		row.RegisterField("b2hWeapon", dr->b2hWeapon);
		row.RegisterField("unk4a", dr->unknown4a);
		row.RegisterField("housePlacementType", dr->housePlacementType);
		row.RegisterField("heraldry", std::string(reinterpret_cast<const char*>(dr->heraldry), 7));
		row.RegisterField("unk1", dr->unk1);
		row.RegisterField("type", dr->type);
		QueueRowInsert(row);
	}

	DoInsertsForTable("raw_appearances", 250);
}