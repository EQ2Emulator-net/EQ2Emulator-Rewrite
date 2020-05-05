#include "stdafx.h"

#include "LuaChatFunctions.h"
#include "../Spawns/Spawn.h"
#include "../ZoneServer/ZoneServer.h"
#include "../ZoneServer/Client.h"

void LuaChatFunctions::RegisterFunctions(lua_State* state) {
	lua_register(state, "Say", Emu_Lua_Say);
	lua_register(state, "CreateConversation", Emu_Lua_CreateConversation);
	lua_register(state, "AddConversationOption", Emu_Lua_AddConversationOption);
	lua_register(state, "CloseConversation", Emu_Lua_CloseConversation);
	lua_register(state, "StartConversation", Emu_Lua_StartConversation);
	lua_register(state, "StartDialogConversation", Emu_Lua_StartDialogConversation);
	lua_register(state, "PlayFlavor", Emu_Lua_PlayFlavor);
}

int LuaChatFunctions::Emu_Lua_Say(lua_State* state) {
	auto spawn = GetLuaSpawn(state, 1);
	if (!spawn) {
		return 0;
	}

	auto zone = spawn->GetZone();
	if (!zone) {
		return 0;
	}

	std::string text = GetLuaString(state, 2);

	zone->chat.HandleSay(text.c_str(), spawn);
	return 0;
}

int LuaChatFunctions::Emu_Lua_CreateConversation(lua_State* state) {
	PushLuaConversation(state);
	return 1;
}

int LuaChatFunctions::Emu_Lua_AddConversationOption(lua_State* state) {
	LuaConversation* conv = GetLuaConversation(state, 1);
	if (!conv) {
		LuaError("Lua AddConversationOption error: conversation not valid!");
		return 0;
	}

	conv->options.emplace_back();
	ConversationOption& option = conv->options.back();
	option.option = GetLuaString(state, 2);
	option.function = GetLuaString(state, 3);
	return 0;
}

int LuaChatFunctions::Emu_Lua_CloseConversation(lua_State* state) {
	auto npc = GetLuaSpawn(state, 1);

	if (!npc) {
		LuaError("Lua CloseConversation error: npc not valid!");
		return 0;
	}

	auto player = GetLuaSpawn(state, 2);

	if (!player || !player->IsPlayer()) {
		LuaError("Lua CloseConversation error: player not valid!");
		return 0;
	}

	auto zone = player->GetZone();
	if (!zone) {
		return 0;
	}

	auto client = zone->GetClientForSpawn(player);
	if (client) {
		
	}

	return 0;
}

int LuaChatFunctions::Emu_Lua_StartConversation(lua_State* state) {
	LuaConversation* conv = GetLuaConversation(state, 1);
	if (!conv) {
		LuaError("Lua StartConversation error: invalid conversation");
		return 0;
	}

	auto speaker = GetLuaSpawn(state, 2);

	if (!speaker) {
		LuaError("Lua StartConversation error: invalid speaker");
		return 0;
	}

	auto player = GetLuaSpawn(state, 3);

	if (!player || !player->IsPlayer()) {
		LuaError("Lua StartConversation error: invalid player");
		return 0;
	}

	auto zone = player->GetZone();
	if (!zone) {
		return 0;
	}

	auto client = zone->GetClientForSpawn(player);

	if (!client) {
		return 0;
	}

	OpenDialogParams p;
	p.scriptSpawn = speaker;
	p.options = conv->options;
	p.text = GetLuaString(state, 4);
	p.mp3 = GetLuaString(state, 5);
	p.key1 = GetLuaUInt32(state, 6);
	p.key2 = GetLuaUInt32(state, 7);

	client->chat.DisplaySpawnDialog(p, 1);
	return 0;
}

int LuaChatFunctions::Emu_Lua_StartDialogConversation(lua_State* state) {
	LuaConversation* conv = GetLuaConversation(state, 1);
	if (!conv) {
		LuaError("Lua StartDialogConversation error: invalid conversation");
		return 0;
	}

	uint8_t type = static_cast<uint8_t>(GetLuaUInt32(state, 2));

	if (type != 1 && type != 3) {
		LuaError("StartDialogConversation error: item conversations are not handled yet!");
		return 0;
	}

	auto speaker = GetLuaSpawn(state, 3);

	if (!speaker) {
		LuaError("Lua StartDialogConversation error: invalid speaker");
		return 0;
	}

	auto player = GetLuaSpawn(state, 4);

	if (!player || !player->IsPlayer()) {
		LuaError("Lua StartDialogConversation error: invalid player");
		return 0;
	}

	auto zone = player->GetZone();
	if (!zone) {
		return 0;
	}

	auto client = zone->GetClientForSpawn(player);

	if (!client) {
		return 0;
	}

	OpenDialogParams p;
	p.scriptSpawn = speaker;
	p.options = conv->options;
	p.text = GetLuaString(state, 5);
	p.mp3 = GetLuaString(state, 6);
	p.key1 = GetLuaUInt32(state, 7);
	p.key2 = GetLuaUInt32(state, 8);

	client->chat.DisplaySpawnDialog(p, type);
	return 0;
}

int LuaChatFunctions::Emu_Lua_PlayFlavor(lua_State* state) {
	return 0;
}