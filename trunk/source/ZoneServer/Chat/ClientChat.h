#pragma once

#include "ClientChatFilters.h"
#include <string>
#include "../../common/Packets/PacketElements/PacketEQ2Color.h"

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

class ClientChat {
public:
	ClientChat(class Client& client);
	~ClientChat() = default;

	ClientChatFilters filters;

	void HearChat(const HearChatParams& params);
	void OnScreenMessage(uint8_t msgType, const std::string& message, const std::string& sound, float durationSecs, EQ2Color color);
	void DisplayText(const char* filter, const std::string& message, uint8_t onscreenMsgType = 0xff, bool bOnscreen = false, const std::string& unkString = "");
	
	void SendSimpleGameMessage(const char* msg);

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
};