#include "stdafx.h"

#include "ClientChat.h"
#include "ChatFilterLookup.h"
#include "../Packets/OP_HearChatCmd_Packet.h"
#include "../Packets/OP_OnscreenMsgMsg_Packet.h"
#include "../Packets/OP_DisplayTextCmd_Packet.h"
#include "../ZoneServer/Client.h"

extern ChatFilterLookup g_chatFilterLookup;

ClientChat::ClientChat(Client& _client): client(_client), sayFilter(0xFF), shoutFilter(0xFF), groupFilter(0xFF), guildFilter(0xFF), raidFilter(0xFF), 
outOfCharacterFilter(0xFF), tellFilter(0xFF), customChannelFilter(0xFF){
}

uint8_t ClientChat::GetFilterByName(const std::string name) {
	uint8_t ret = 0xFF;

	auto InitializeFilter = [this, &name, &ret](uint8_t& out) {
		if (out == 0xFF) {
			out = g_chatFilterLookup.GetChatFilterID(name.c_str(), client.GetVersion());
		}
		ret = out;
	};

	//Check if we have this filter cached before searching
	if (name == "Say") {
		InitializeFilter(sayFilter);
	}
	else if (name == "Custom Channel Text") {
		InitializeFilter(customChannelFilter);
	}
	else if (name == "Shout") {
		InitializeFilter(shoutFilter);
	}
	else if (name == "Group Say") {
		InitializeFilter(groupFilter);
	}
	else if (name == "Raid Say") {
		InitializeFilter(raidFilter);
	}
	else if (name == "Guild Say") {
		InitializeFilter(guildFilter);
	}
	else if (name == "Out of Character") {
		InitializeFilter(outOfCharacterFilter);
	}
	else if (name == "Tell") {
		InitializeFilter(tellFilter);
	}
	else {
		InitializeFilter(ret);
	}

	return ret;
}

void ClientChat::HearChat(const HearChatParams& params) {
	uint8_t filter = GetFilterByName(params.chatFilterName);
	if (filter == 0xFF) {
		LogWarn(LOG_CHAT, 0, "Could not find a filter for channel %s version %u!", params.chatFilterName, client.GetVersion());
		return;
	}

	if (!filters.IsChannelEnabled(filter)) {
		return;
	}

	OP_HearChatCmd_Packet p(client.GetVersion(), params);
	p.chatFilterID = filter;
	client.QueuePacket(p);
}

void ClientChat::OnScreenMessage(uint8_t msgType, const std::string& message, const std::string& sound, float durationSecs, EQ2Color color) {
	OP_OnscreenMsgMsg_Packet p(client.GetVersion());
	p.msgType = msgType;
	p.text = message;
	p.sound = sound;
	p.durationSecs = durationSecs;
	p.color = color;
	client.QueuePacket(p);
}

void ClientChat::DisplayText(const char* filterName, const std::string& message, uint8_t onscreenMsgType, bool bOnscreen, const std::string& onscreenSound) {
	uint8_t filter = GetFilterByName(filterName);
	OP_DisplayTextCmd_Packet p(client.GetVersion());
	p.chatFilterID = filter;
	p.bOnscreen = bOnscreen;
	p.onscreenMsgType = onscreenMsgType;
	p.text = message;
	//This string is only in the packet if bOnscreen is true so thinking maybe it's the sound string in the OnscreenMsgMsg?
	p.onscreenSound = onscreenSound;
	client.QueuePacket(p);
}