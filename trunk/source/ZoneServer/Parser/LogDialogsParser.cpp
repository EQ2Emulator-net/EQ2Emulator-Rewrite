#include "stdafx.h"

#include "LogDialogsParser.h"
#include "../../common/BinaryStream.h"
#include "../Packets/OP_CreateSignWidgetCmd_Packet.h"
#include "../Packets/OP_DialogOpenCmd_Packet.h"
#include "../Packets/OP_ZoneInfoMsg_Packet.h"
#include "../Packets/OP_PlayVoiceCmd_Packet.h"
#include "../Packets/OP_DialogSelectMsg_Packet.h"
#include "../Packets/OP_DialogCloseMsg_Packet.h"
#include "../Packets/OP_HearPlayFlavorCmd_Packet.h"

bool OP_ZoneInfoMsg_Packet::bBaseInfoOnly = false;

LogDialogsParser::LogDialogsParser(PacketLog& log, ParserDatabase& db) : LogParser(log, db) {
	BuildSpawnNamesMap();
	ProcessZones();
	ProcessDialogs();
	ProcessPlayVoiceCmd();
	ProcessSelections();
	ProcessPlayFlavorCmd();

	//Do tables that other tables depend on first
	DoInsertsForTable("dialog_voiceovers", 100, true);
	DoInsertsForTable("dialog_text", 50, true);
	DoInsertsForTable("dialog_npcs", 100, true);
	DoInsertsForTable("dialog_flavors", 100, true);
	DoInsertsForTable("dialog_play_flavors", 100, true);
	DoInsertsForTable("dialog_play_voices", 100, true);
	DoInsertsForTable("dialogs", 100, true);
	DoInsertsForTable("dialog_responses", 100, true);

	std::ostringstream ss;
	//Update response entries using dialog selections
	for (auto& itr : selections) {
		uint32_t parentDialog = itr.first.first;
		uint32_t index = itr.first.second;
		uint32_t nextDialog = itr.second;
		
		ss << "UPDATE dialog_responses SET next_dialog_id = " << nextDialog
			<< " WHERE parent_dialog_id = " << parentDialog << " AND `index` = " << index << ";\n";
	}

	std::string updates = ss.str();
	if (!updates.empty()) {
		database.QuerySimple(updates.c_str(), (unsigned long)updates.size());
	}
}

void LogDialogsParser::BuildSpawnNamesMap() {
	OP_CreateGhostCmd_Packet::bUnpackData = false;
	auto spawns = log.FindPackets<OP_CreateGhostCmd_Packet>();
	{
		auto widgets = log.FindPackets<OP_CreateWidgetCmd_Packet, OP_CreateGhostCmd_Packet>();
		auto signs = log.FindPackets<OP_CreateSignWidgetCmd_Packet, OP_CreateGhostCmd_Packet>();

		std::move(widgets.begin(), widgets.end(), std::back_inserter(spawns));
		std::move(signs.begin(), signs.end(), std::back_inserter(spawns));
	}
	OP_CreateGhostCmd_Packet::bUnpackData = true;

	for (auto& itr : spawns) {
		auto& footer = itr.second->footer;
		uint32_t id = itr.second->header.spawn_id;
		
		spawnNames[id][itr.first] = footer.titleStruct.SpawnTitleStruct::name;
	}
}

std::optional<std::string> LogDialogsParser::GetSpawnName(uint32_t id, uint32_t line) {
	std::optional<std::string> ret;

	auto f = spawnNames.find(id);
	if (f == spawnNames.end()) return ret;

	
	for (auto& itr : f->second) {
		if (itr.first > line && ret) break;
		ret.emplace(itr.second);
	}

	return ret;
}

void LogDialogsParser::ProcessDialogs() {
	auto pDialogs = log.FindPackets<OP_DialogOpenCmd_Packet>();

	for (auto& itr : pDialogs) {
		AddDialog(itr.first, itr.second.get());
	}
}

uint32_t LogDialogsParser::AddVoiceover(const Voiceover& vo) {
	uint32_t hash = crc32('VOVR', reinterpret_cast<const Bytef*>(vo.file.c_str()), (uInt)vo.file.length());
	
	auto f = voiceovers.find(hash);
	if (f == voiceovers.end()) {
		DatabaseRow row;
		row.m_tableName = "dialog_voiceovers";
		row.RegisterField("id", hash);
		row.RegisterField("file", vo.file);
		row.RegisterField("key1", vo.key1);
		row.RegisterField("key2", vo.key2);
		QueueRowInsert(row);
		voiceovers.emplace(hash);
	}

	return hash;
}

void LogDialogsParser::ProcessZones() {
	OP_ZoneInfoMsg_Packet::bBaseInfoOnly = true;
	auto zones = log.FindPackets<OP_ZoneInfoMsg_Packet>();
	OP_ZoneInfoMsg_Packet::bBaseInfoOnly = false;
	for (auto& itr : zones) {
		std::string zoneName = itr.second->zone_desc;
		if (zoneName[0] == ':') {
			//Check for string mapping and strip it
			size_t end = zoneName.find(':', 1);
			if (end != string::npos) {
				zoneName = zoneName.substr(end + 1);
			}
		}
		zoneList[itr.first] = zoneName;
	}
}

std::string LogDialogsParser::FindZoneForLine(uint32_t line) {
	std::string ret;
	for (auto& itr : zoneList) {
		if (itr.first < line) {
			ret = itr.second;
		}
		else break;
	}
	return ret;
}

void LogDialogsParser::AddDialog(uint32_t line, OP_DialogOpenCmd_Packet* p) {
	Dialog d;

	Voiceover vo;
	vo.file = p->voiceFile;
	vo.key1 = p->key1;
	vo.key2 = p->key2;

	d.voiceoverID = AddVoiceover(vo);

	//If we don't know about this spawn somehow don't try and save this dialog.
	//Only seen this one time.
	uint32_t npc;
	if (p->spawnID == -1) {
		npc = AddNPC("PLAYERDIALOG", FindZoneForLine(line));
	}
	else {
		auto spawnName = GetSpawnName(p->spawnID, line);
		if (!spawnName) {
			return;
		}
		npc = AddNPC(*spawnName, FindZoneForLine(line));
	}


	uint32_t title = AddText(p->title);
	uint32_t msg = AddText(p->message);
	d.npcID = npc;

	uint32_t dialogID;
	{
		std::ostringstream ss;

		size_t cnt = p->responseArray.size();
		ss << "NPC:" << d.npcID << "VO:" << d.voiceoverID << "TITLE:" << title << "MSG : " << msg << "RCNT : " << cnt;

		int i = 0;
		for (auto& itr : p->responseArray) {
			uint32_t text = AddText(itr);
			ss << "R" << i << ":" << text;
			i++;
			d.responses.push_back(text);
		}

		std::string buf = ss.str();
		dialogID = crc32('DLOG', reinterpret_cast<const Bytef*>(buf.c_str()), (uInt)buf.length());
	}
	
	d.title = title;
	d.msg = msg;
	d.bCloseable = p->bCloseable;
	d.responseColor = p->responseColor;
	d.unknown = p->unknown;
	//Keep a reference of the line this came from for dialog selections
	DialogCacheEntry de;
	de.clientConversationID = p->conversationID;
	de.dialogID = dialogID;
	de.npcID = npc;
	dialogLines[line] = de;

	if (dialogs.count(dialogID) == 0) {
		DatabaseRow row;
		row.m_tableName = "dialogs";
		row.RegisterField("id", dialogID);
		row.RegisterField("npc_id", d.npcID);
		row.RegisterField("voiceover_id", d.voiceoverID);
		if (d.title == 0) {
			row.RegisterField("title_text_id", SQLNull());
		}
		else {
			row.RegisterField("title_text_id", d.title);
		}
		if (d.msg == 0) {
			row.RegisterField("msg_text_id", SQLNull());
		}
		else {
			row.RegisterField("msg_text_id", d.msg);
		}
		row.RegisterField("closeable", d.bCloseable);
		row.RegisterField("unknown", d.unknown);
		row.RegisterField("response_color", d.responseColor);
		row.RegisterField("log_id", log.log_id);
		QueueRowInsert(row);

		int i = 0;
		for (auto& itr : d.responses) {
			std::ostringstream ss;
			ss << "DPAR:" << dialogID << "IND:" << i << "TEXT:" << itr;
			std::string s = ss.str();

			uint32_t hash = crc32('DRSP', reinterpret_cast<const Bytef*>(s.c_str()), (uInt)s.size());

			DatabaseRow row;
			row.m_tableName = "dialog_responses";
			row.RegisterField("id", hash);
			row.RegisterField("parent_dialog_id", dialogID);
			row.RegisterField("index", i++);
			row.RegisterField("text_id", itr);
			QueueRowInsert(row);
		}

		dialogs.insert(dialogID);
	}
}

uint32_t LogDialogsParser::AddNPC(std::string name, std::string zone) {
	std::ostringstream ss;
	ss << "NAME:" << name << "ZONE:" << zone;

	std::string s = ss.str();
	uint32_t hash = crc32('DNPC', reinterpret_cast<const Bytef*>(s.c_str()), (uInt)s.size());

	if (npcs.count(hash) == 0) {
		DatabaseRow row;
		row.m_tableName = "dialog_npcs";
		row.RegisterField("id", hash);
		row.RegisterField("zone", zone);
		row.RegisterField("name", name);
		QueueRowInsert(row);

		npcs.insert(hash);
	}

	return hash;
}

void LogDialogsParser::ProcessPlayVoiceCmd() {
	auto hearVoice = log.FindPackets<OP_PlayVoiceCmd_Packet>();

	for (auto& itr : hearVoice) {
		uint32_t line = itr.first;
		auto& p = itr.second;

		Voiceover vo;
		vo.file = p->voiceFile;
		vo.key1 = p->key1;
		vo.key2 = p->key2;

		uint32_t voID = AddVoiceover(vo);

		if (p->spawnID == -1) {
			//only seen this in one packet out of a ton of logs, just save the key/file
			continue;
		}

		auto spawnName = GetSpawnName(p->spawnID, line);

		if (!spawnName) {
			//another rarity, spawn id we havent seen yet in this log
			continue;
		}

		uint32_t npcID = AddNPC(*spawnName, FindZoneForLine(line));
		uint32_t garb = AddText(p->garbled);

		std::ostringstream ss;
		ss << "NPC:" << npcID << "VO:" << voID << "GARB:" << p->garbled;

		std::string s = ss.str();
		uint32_t hash = crc32('PLYV', reinterpret_cast<const Bytef*>(s.c_str()), (uInt)s.size());
		if (playVoiceList.count(hash) == 0) {
			DatabaseRow row;
			row.m_tableName = "dialog_play_voices";

			row.RegisterField("id", hash);
			row.RegisterField("npc_id", npcID);
			row.RegisterField("voiceover_id", voID);
			row.RegisterField("language", p->language);
			if (garb == 0) {
				row.RegisterField("garbled_text_id", SQLNull());
			}
			else {
				row.RegisterField("garbled_text_id", garb);
			}
			row.RegisterField("log_id", log.log_id);
			QueueRowInsert(row);

			playVoiceList.insert(hash);
		}
	}
}

uint32_t LogDialogsParser::AddText(std::string t) {
	if (t.empty()) {
		return 0;
	}

	uint32_t hash = crc32('DTXT', reinterpret_cast<const Bytef*>(t.c_str()), (uInt)t.size());
	if (textList.count(hash) == 0) {
		DatabaseRow row;
		row.m_tableName = "dialog_text";
		row.RegisterField("id", hash);
		row.RegisterField("text", t);
		QueueRowInsert(row);
		textList.insert(hash);
	}
	return hash;
}

void LogDialogsParser::ProcessSelections() {
	auto sels = log.FindPackets<OP_DialogSelectMsg_Packet>();
	auto closes = log.FindPackets<OP_DialogCloseMsg_Packet>();

	auto FindCloseForConversation = [&closes](uint32_t openLine, uint32_t id) -> uint32_t {
		for (auto& itr : closes) {
			uint32_t closeLine = itr.first;
			if (closeLine < openLine) continue;

			if (itr.second->conversationID == id) {
				return closeLine;
			}
		}

		return 0xFFFFFFFF;
	};

	for (auto& itr : sels) {
		uint32_t selLine = itr.first;
		auto& sel = itr.second;
		uint32_t convoID = sel->conversationID;
		uint32_t selectedIndex = sel->selection;

		uint32_t parentDialogID = 0;
		uint32_t parentDialogLine = 0;
		uint32_t parentDialogNpc = 0;

		//Find the previous dialog open for this conversation
		for (auto& itr : dialogLines) {
			uint32_t openLine = itr.first;
			DialogCacheEntry& de = itr.second;
			uint32_t openConvoID = de.clientConversationID;
			uint32_t openDialogID = de.dialogID;

			if (openLine > selLine) break;

			if (openConvoID == convoID) {
				parentDialogID = openDialogID;
				parentDialogLine = openLine;
				parentDialogNpc = de.npcID;
			}
		}

		if (parentDialogID != 0) {
			//Figure out if there was a response to this selection
			uint32_t closeLine = FindCloseForConversation(parentDialogLine, convoID);
			auto f = dialogLines.find(parentDialogLine);
			while (++f != dialogLines.end()) {
				uint32_t nextOpenLine = f->first;
				DialogCacheEntry& de = f->second;
				uint32_t nextOpenConvoID = de.clientConversationID;
				uint32_t nextOpenDialogID = de.dialogID;

				if (nextOpenLine > closeLine) {
					break;
				}

				if (nextOpenConvoID == convoID && de.npcID == parentDialogNpc) {
					selections[std::make_pair(parentDialogID, selectedIndex)] = nextOpenDialogID;
					break;
				}
			}
		}
	}
}

void LogDialogsParser::ProcessPlayFlavorCmd() {
	auto flavors = log.FindPackets<OP_HearPlayFlavorCmd_Packet>();

	for (auto& itr : flavors) {
		uint32_t line = itr.first;
		auto& p = itr.second;

		//Build a "flavor" entry (the actual chat/voiceover/emote data)
		Voiceover vo;
		vo.file = p->mp3;
		vo.key1 = p->key1;
		vo.key2 = p->key2;
		uint32_t voID = AddVoiceover(vo);

		uint32_t textID = AddText(p->text);
		uint32_t emoteTextID = AddText(p->emoteText);

		std::ostringstream ss;
		ss << "VO:" << voID << "TEXT:" << textID << "ETEXT:" << emoteTextID
			<< "LANG:" << static_cast<uint32_t>(p->language) << "BUND:" << (p->bUnderstood ? 1 : 0)
			<< "EMOTE:" << p->emote << "UNK4:" << static_cast<uint32_t>(p->unknown4);

		//make a hash/id
		std::string s = ss.str();
		uint32_t flavorID = crc32('FLAV', reinterpret_cast<const Bytef*>(s.c_str()), (uInt)s.size());

		if (flavorList.count(flavorID) == 0) {
			DatabaseRow row;
			row.m_tableName = "dialog_flavors";
			row.RegisterField("id", flavorID);
			if (voID != 0) {
				row.RegisterField("voiceover_id", voID);
			}
			else {
				row.RegisterField("voiceover_id", SQLNull());
			}
			if (textID != 0) {
				row.RegisterField("text_id", textID);
			}
			else {
				row.RegisterField("text_id", SQLNull());
			}
			row.RegisterField("language", p->language);
			row.RegisterField("understood", p->bUnderstood);
			row.RegisterField("emote", p->emote);
			if (emoteTextID != 0) {
				row.RegisterField("emote_text_id", emoteTextID);
			}
			else {
				row.RegisterField("emote_text_id", SQLNull());
			}
			row.RegisterField("unknown4", p->unknown4);
			row.RegisterField("log_id", log.log_id);
			QueueRowInsert(row);

			flavorList.insert(flavorID);
		}

		//Now the play flavor
		uint32_t npc = AddNPC(p->speakerName, FindZoneForLine(line));

		ss.str("");
		ss << "NPC:" << npc << "FLAV:" << flavorID;
		s = ss.str();

		uint32_t playID = crc32('PLYF', reinterpret_cast<const Bytef*>(s.c_str()), (uInt)s.size());

		if (playFlavorList.count(playID) == 0) {
			DatabaseRow row;
			row.m_tableName = "dialog_play_flavors";
			row.RegisterField("id", playID);
			row.RegisterField("npc_id", npc);
			row.RegisterField("flavor_id", flavorID);
			row.RegisterField("log_id", log.log_id);
			QueueRowInsert(row);
			playFlavorList.insert(playID);
		}
	}
}