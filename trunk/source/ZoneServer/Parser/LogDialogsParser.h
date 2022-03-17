#pragma once

#include "../../common/Parser/LogParser.h"
#include <map>
#include <string>
#include <set>
#include <optional>

class LogDialogsParser : public LogParser {
public:
	LogDialogsParser(PacketLog& log, class ParserDatabase& db);
	~LogDialogsParser() = default;

private:
	void BuildSpawnNamesMap();
	std::optional<std::string> GetSpawnName(uint32_t id, uint32_t line);
	//map<spawnID, map<line, name> >
	std::map<uint32_t, std::map<uint32_t, std::string>> spawnNames;

	//map<line, zone name> zones
	std::map<uint32_t, std::string> zoneList;
	void ProcessZones();
	std::string FindZoneForLine(uint32_t line);

	//map<line, std::pair<client conversation id, emu conversation hash> >
	std::map<uint32_t, std::pair<uint32_t, uint32_t> > dialogLines;
	void ProcessDialogs();

	struct Voiceover {
		std::string file;
		uint32_t key1;
		uint32_t key2;
	};

	std::set<uint32_t> voiceovers;
	uint32_t AddVoiceover(const Voiceover& v);

	struct Dialog {
		uint32_t voiceoverID;
		uint32_t npcID;
		uint32_t title;
		uint32_t msg;
		std::vector<uint32_t> responses;
		bool bCloseable;
		uint8_t responseColor;
		uint8_t unknown;
	};

	//set<id>
	std::set<uint32_t> dialogs;
	void AddDialog(uint32_t line, class OP_DialogOpenCmd_Packet* p);

	struct DialogNPC {
		std::string name;
		std::string zone;
	};

	std::set<uint32_t> npcs;
	uint32_t AddNPC(std::string name, std::string zone);

	void ProcessPlayVoiceCmd();
	std::set<uint32_t> playVoiceList;

	void ProcessSelections();
	//map<pair<parentDialogID, index>, nextDialogID>
	std::map<std::pair<uint32_t, uint32_t>, uint32_t> selections;

	void ProcessPlayFlavorCmd();
	std::set<uint32_t> playFlavorList;
	std::set<uint32_t> flavorList;

	uint32_t AddText(std::string t);
	std::set<uint32_t> textList;
};