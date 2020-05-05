#pragma once

#include "ClientChatFilters.h"
#include <string>
#include "../../common/Packets/PacketElements/PacketEQ2Color.h"
#include "../Lua/LuaUserData.h"

class Spawn;

struct HearChatParams {
	HearChatParams() : language(0), bFromNPC(false), bShowBubble(false), bUnderstood(true),
		fromSpawnID(0xFFFFFFFF), toSpawnID(0xFFFFFFFF), unknownb(1), unknownd(2) {}
	uint8_t language;
	bool bFromNPC;
	bool bShowBubble;
	bool bUnderstood;
	uint32_t fromSpawnID;
	uint32_t toSpawnID;
	uint32_t unknownb;
	uint32_t unknownd;
	std::string fromName;
	std::string toName;
	std::string message;
	std::string channelName;
	const char* chatFilterName;
};

struct PlayFlavorParams {
	PlayFlavorParams() : key1(0), key2(0), language(0), bUnderstood(true), fromSpawnID(0xFFFFFFFF), toSpawnID(0xFFFFFFFF) {}
	uint32_t fromSpawnID;
	uint32_t toSpawnID;
	std::string mp3;
	uint32_t key1;
	uint32_t key2;
	std::string speakerName;
	std::string targetName;
	std::string text;
	std::string emoteText;
	std::string emote;
	uint8_t language;
	bool bUnderstood;
};

struct OpenDialogParams {
	std::shared_ptr<Spawn> scriptSpawn;
	std::string text;
	std::vector<ConversationOption> options;
	std::string mp3;
	uint32_t key1;
	uint32_t key2;
};

class ClientChat {
public:
	ClientChat(class Client& client);
	~ClientChat() = default;

	ClientChatFilters filters;

	void HearChat(const HearChatParams& params);
	void OnScreenMessage(uint8_t msgType, const std::string& message, const std::string& sound, float durationSecs, EQ2Color color);
	void DisplayText(const char* filter, const std::string& message, uint8_t onscreenMsgType = 0xff, bool bOnscreen = false, const std::string& unkString = "");
	void HearPlayFlavor(const PlayFlavorParams& params);
	
	void SendSimpleGameMessage(const char* msg);

	//Conversations
	void DisplaySpawnDialog(const OpenDialogParams& params, uint8_t type);
	void HandleDialogSelection(uint32_t conversationID, uint32_t selection);

private:
	Client& client;

	uint8_t GetFilterByName(const std::string name);

	//Some cached channels that are used somewhat frequently
	uint8_t sayFilter;
	uint8_t shoutFilter;
	uint8_t groupFilter;
	uint8_t guildFilter;
	uint8_t raidFilter;
	uint8_t outOfCharacterFilter;
	uint8_t tellFilter;
	uint8_t customChannelFilter;

	struct CurrentDialog {
		std::vector<std::string> choiceFunctions;
		std::weak_ptr<Spawn> scriptSpawn;
	};

	//Dialog/Conversations
	uint32_t nextConversationID;
	std::map<uint32_t, CurrentDialog> conversations;
};