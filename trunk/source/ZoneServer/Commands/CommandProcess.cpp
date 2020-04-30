#include "stdafx.h"

#include "CommandProcess.h"
#include "../ZoneServer/Client.h"
#include "../../common/Separator.h"
#include "../Database/ZoneDatabase.h"
#include "../Packets/OP_EqSetControlGhostCmd_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../Packets/OP_TeleportWithinZoneNoReloadMsg_Packet.h"
#include "../Controllers/PlayerController.h"
#include "../Spawns/Entity.h"
#include "../ZoneServer/ZoneOperator.h"
#include "../Packets/OP_ChangeServerControlFlagMsg.h"
#include "../Packets/OP_HearChatCmd_Packet.h"
#include "../ZoneServer/ZoneServer.h"

extern ZoneOperator g_zoneOperator;

CommandProcess::CommandProcess() {
	RegisterCommands();
}

void CommandProcess::RegisterCommands() {
	RegisterCommandHandler(214, CommandSpeed);
	RegisterCommandHandler(206, CommandMove);
	RegisterCommandHandler(247, CommandTest);
	RegisterCommandHandler(205, CommandZone);
	RegisterCommandHandler(228, CommandFlymode);
	RegisterCommandHandler(15, CommandSay);
	RegisterCommandHandler(28, CommandShout);
	RegisterCommandHandler(14, CommandEmote);
	RegisterCommandHandler(30, CommandOOC);
	RegisterCommandHandler(402, CommandGrid);
	RegisterCommandHandler(219, CommandSpawnSet);

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
	auto controller = client->GetController();

	auto target = controller->GetTarget();

	if (!target) {
		return;
	}

	uint16_t spawnID = client->GetIndexForSpawn(target);

	if (spawnID == 0) {
		//should never happen
		return;
	}

	SpawnPositionStruct pos = *target->GetPosStruct();
	pos.actorStopRange = 10.f;
	pos.faceActorID = spawnID;
	target->SetSpawnPositionData(pos, Timer::GetServerTime());
}

void CommandProcess::CommandZone(const std::shared_ptr<Client>& client, Separator& sep) {
	//Add more checks/syntax for this, like zone names/instance ids
	if (!sep.IsNumber(0)) {
		return;
	}

	uint32_t zone_id = sep.GetUInt32(0);

	if (!g_zoneOperator.RequestZoneTransfer(client, zone_id, 0)) {
		client->chat.SendSimpleGameMessage("\\#FF0000 You're already trying to zone. Please wait...");
		return;
	}
}

void CommandProcess::CommandFlymode(const std::shared_ptr<Client>& client, Separator& sep) {
	if (!sep.IsNumber(0)) {
		return;
	}

	OP_ChangeServerControlFlagMsg_Packet p(client->GetVersion());
	p.positionState2 = 32;
	p.value = sep.GetUInt32(0) != 0;
	client->QueuePacket(p);
}

void CommandProcess::CommandSay(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() < 1) {
		return;
	}

	auto zone = client->GetZone();

	if (!zone) {
		return;
	}

	auto sender = client->GetController()->GetControlled();

	if (!sender) {
		return;
	}

	zone->chat.HandleSay(sep.GetInputString().c_str(), sender);
}

void CommandProcess::CommandShout(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() < 1) {
		return;
	}

	auto zone = client->GetZone();

	if (!zone) {
		return;
	}

	auto sender = client->GetController()->GetControlled();

	if (!sender) {
		return;
	}

	zone->chat.HandleShout(sep.GetInputString().c_str(), sender);
}

void CommandProcess::CommandEmote(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() < 1) {
		return;
	}

	auto zone = client->GetZone();

	if (!zone) {
		return;
	}

	auto controller = client->GetController();

	auto sender = controller->GetControlled();

	if (!sender) {
		return;
	}
	
	//TODO: need to handle targets etc in this
	zone->chat.HandleEmoteChat(sep.GetInputString().c_str(), sender);
}

void CommandProcess::CommandOOC(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() < 1) {
		return;
	}

	auto zone = client->GetZone();

	if (!zone) {
		return;
	}

	auto sender = client->GetController()->GetControlled();

	if (!sender) {
		return;
	}

	zone->chat.HandleOutOfCharacter(sep.GetInputString().c_str(), sender);
}

void CommandProcess::CommandGrid(const std::shared_ptr<Client>& client, Separator& sep) {
	std::shared_ptr<Entity> player = client->GetController()->GetControlled();
	if (!player) {
		return;
	}

	uint32_t grid = player->GetPosStruct()->grid_id;
	std::pair<int32_t, int32_t> cellCoord = player->GetCellCoordinates();

	client->chat.SendSimpleGameMessage("Test");
	
	LogDebug(LOG_PLAYER, 0, "Grid ID: %u\nCell Coordinates: %i, %i", grid, cellCoord.first, cellCoord.second);
	char temp[64];
	sprintf(temp, "Grid ID: %u", grid);
	client->chat.SendSimpleGameMessage(temp);
	sprintf(temp, "Cell Coordinates: %i, %i", cellCoord.first, cellCoord.second);
	client->chat.SendSimpleGameMessage(temp);
}