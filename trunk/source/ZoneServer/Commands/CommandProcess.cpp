#include "stdafx.h"

#include "CommandProcess.h"
#include "../ZoneServer/Client.h"
#include "../../common/Separator.h"
#include "../Database/ZoneDatabase.h"
#include "../Packets/OP_EqSetControlGhostCmd_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../Packets/OP_TeleportWithinZoneNoReloadMsg_Packet.h"
#include "../Controllers/PlayerController.h"
#include "../Spawns/Spawn.h"
#include "../Packets/OP_ChangeZoneMsg_Packet.h"
#include "../ZoneServer/ZoneOperator.h"

extern ZoneOperator z;

CommandProcess::CommandProcess() {
	RegisterCommands();
}

void CommandProcess::RegisterCommands() {
	RegisterCommandHandler(214, CommandSpeed);
	RegisterCommandHandler(206, CommandMove);
	RegisterCommandHandler(247, CommandTest);
	RegisterCommandHandler(205, CommandZone);
}

void CommandProcess::RegisterCommandHandler(uint32_t handler_id, CommandHandler_t handler) {
	assert(handlers.count(handler_id) == 0);
	handlers[handler_id] = handler;
}

bool ZoneDatabase::LoadCommands(CommandProcess& process) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT id, command, parent_command, required_status FROM commands ORDER BY parent_command, id ASC");
	if (!ret)
		return ret;

	uint32_t count = 0;
	while (result.Next()) {
		process.AddCommand(result.GetUInt32(0), result.GetString(1), result.GetUInt32(2), result.GetString(3));
		count++;
	}

	LogDebug(LOG_COMMAND, 0, "Loaded %u commands", count);

	return ret;
}

void CommandProcess::AddCommand(uint32_t handler_id, const std::string& cmd, uint32_t parent_id, const std::string& required_status) {
	CommandHandler_t handler = nullptr;
	{
		//Find the handler if we have one registered
		auto itr = handlers.find(handler_id);
		if (itr != handlers.end()) handler = itr->second;
	}

	//Save this command
	auto command = make_shared<CommandInfo>();
	command->handler = handler;
	command->token.Parse(required_status);
	command->cmdText = cmd;

	//This map is for ease of building child commands
	commandsParentLookup[handler_id] = command;

	if (parent_id == 0) {
		//This is a root command with no parent
		rootCommands[static_cast<uint32_t>(rootCommands.size())] = command;
	}
	else {
		auto itr = commandsParentLookup.find(parent_id);
		//If this assertion fails it means a child command was loaded before its parent
		assert(itr != commandsParentLookup.end());

		//Store this command in its direct parent's sub command list
		itr->second->subCommands[cmd] = command;
	}
}

void CommandProcess::ProcessCommand(const std::shared_ptr<Client>& client, uint32_t commandIndex, const std::string& args) {
	//First figure out which command this is
	auto itr = rootCommands.find(commandIndex);
	if (itr == rootCommands.end()) {
		LogError(LOG_COMMAND, 0, "Unknown command index %u received!", commandIndex);
		return;
	}
	
	std::shared_ptr<CommandInfo> cmd = itr->second;
	
	//This string is for debugging purposes
	std::ostringstream fullCommand;
	fullCommand << cmd->cmdText;

	//Is this a sub command?
	Separator sep(args);
	while (sep.GetSize()) {
		auto itr = cmd->subCommands.find(sep.GetString(0));
		if (itr == cmd->subCommands.end()) {
			break;
		}

		//This is a sub command! Update our currently found command and continue searching for child commands
		cmd = itr->second;
		fullCommand << ' ' << cmd->cmdText;
		sep.DropFirstArg();
		//Since we bumped all arg indices up we don't need to increment for the next check
	}

	if (!cmd->handler) {
		LogDebug(LOG_COMMAND, 0, "Player tried to use a command with an undefined handler : %s, args:\n%s", fullCommand.str().c_str(), args.c_str());
		return;
	}

	//if (!cmd->token.MatchesAllTokens(PLAYERTOKENS)) {
	//	PLAYER->SENDMESSAGE("You do not meet the requirements to use this command.");
	//}

	cmd->handler(client, sep);
}

void CommandProcess::CommandSpeed(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() == 0 || !sep.IsNumber(0)) {
		//ERROR MESSAGE TO PLAYER INCORRECT SYNTAX
		return;
	}

	//Obviously we need proper handling for sending this packet rather than just sending it straight, need to set char sheet etc
	float speed = sep.GetFloat(0);

	OP_EqSetControlGhostCmd_Packet packet(client->GetVersion());
	packet.spawn_id = 0xFFFFFFFF;
	packet.speed = speed;
	packet.air_speed = speed;
	packet.size = 0.8f;
	client->QueuePacket(packet);
}

void CommandProcess::SendCommandList(const std::shared_ptr<Client>& client) {
	OP_SetRemoteCmdsMsg_Packet packet(client->GetVersion());
	
	packet.commands_array.reserve(rootCommands.size());

	for (auto& itr : rootCommands) {
		packet.commands_array.emplace_back(client->GetVersion());
		auto& entry = packet.commands_array.back();
		entry.name = itr.second->cmdText;
	}

	client->QueuePacket(packet);
}

void CommandProcess::CommandMove(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() < 3 || !sep.IsNumber(0) || !sep.IsNumber(1) || !sep.IsNumber(2)) {
		//syntax error
		return;
	}

	float x = sep.GetFloat(0);
	float y = sep.GetFloat(1);
	float z = sep.GetFloat(2);

	OP_TeleportWithinZoneNoReloadMsg_Packet packet(client->GetVersion());
	packet.x = x;
	packet.y = y;
	packet.z = z;
	client->QueuePacket(packet);
}

void CommandProcess::CommandTest(const std::shared_ptr<Client>& client, Separator& sep) {
	if (!sep.IsNumber(0)) {
		return;
	}

	auto controller = client->GetController();
	auto target = controller->GetTarget();
	if (!target) {
		return;
	}

	uint32_t shift = sep.GetUInt32(0);

	target->ToggleEntityFlags(1 << shift);
}

void CommandProcess::CommandZone(const std::shared_ptr<Client>& client, Separator& sep) {
	//Add more checks/syntax for this, like zone names/instance ids
	if (!sep.IsNumber(0)) {
		return;
	}

	uint32_t zone_id = sep.GetUInt32(0);

	auto zone = z.AddNewZone(zone_id, 0);

	if (!zone) {
		//invalid zone id;
		LogError(LOG_COMMAND, 0, "Command \"zone\" invalid invalid zone id %u", zone_id);
		return;
	}

	if (zone == client->GetZone()) {
		LogError(LOG_COMMAND, 0, "Command \"zone\" player tried change to a zone they were already in.");
		return;
	}

	PendingClient pc;
	pc.access_code = MakeRandomNumber();
	pc.character_id = client->GetCharacterID();
	pc.instance_id = 0;
	pc.zone_id = zone_id;

	z.AddPendingClient(client->GetAccountID(), pc);

	//Most of this logic will probably be moved to the zoneserver and/or zoneoperator, just a test to see if we can load more zones
	OP_ChangeZoneMsg_Packet p(client->GetVersion());
	p.ip_address = z.GetHostString();
	p.port = z.GetPort();
	p.account_id = client->GetAccountID();
	p.key = pc.access_code;
	client->QueuePacket(p);
}