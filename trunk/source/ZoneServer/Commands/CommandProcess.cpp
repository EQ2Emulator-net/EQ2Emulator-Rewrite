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
#include "../Packets/OP_EqCannedEmoteCmd_Packet.h"
#include "../ZoneServer/MasterZoneLookup.h"

extern ZoneOperator g_zoneOperator;
extern ZoneServer g_zoneServer;
extern MasterZoneLookup g_masterZoneLookup;

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
	RegisterCommandHandler(6, CommandAFK);
	RegisterCommandHandler(17, CommandTell);
	RegisterCommandHandler(224, CommandSpawnDetails);
	RegisterCommandHandler(244, CommandDepop);
	RegisterCommandHandler(245, CommandRepop);
	RegisterCommandHandler(199, CommandSpawn);
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
	//auto controller = client->GetController();

	//auto target = controller->GetTarget();

	//if (!target) {
	//	return;
	//}
}

void CommandProcess::CommandZone(const std::shared_ptr<Client>& client, Separator& sep) {
	//Add more checks/syntax for this, like instance ids
	uint32_t zone_id = 0;
	std::string zone_name = "";

	if (sep.IsNumber(0)) {
		zone_id = sep.GetUInt32(0);

		MasterZoneDetails* details = g_masterZoneLookup.GetZoneInfoByID(zone_id);
		if (!details) {
			std::string message = "\\#FF0000 Unable to locate zone ID: " + to_string(zone_id);
			client->chat.SendSimpleGameMessage(message.c_str());
			return;
		}
	}
	else {
		zone_name = sep.GetString(0);

		MasterZoneDetails* details = g_masterZoneLookup.GetZoneInfoByName(zone_name);
		if (!details) {
			std::string message = "\\#FF0000 Unable to locate zone name: " + zone_name;
			client->chat.SendSimpleGameMessage(message.c_str());
			return;
		}

		zone_id = details->id;
	}

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

void CommandProcess::CommandAFK(const std::shared_ptr<Client>& client, Separator& sep) {
	std::shared_ptr<Entity> player = client->GetController()->GetControlled();
	if (!player) {
		return;
	}

	auto zone = client->GetZone();
	if (!zone) {
		return;
	}

	// TOO DOO: Store away message

	player->ToggleEntityFlags(EntityFlagAfk);

	OP_EqCannedEmoteCmd_Packet* packet = new OP_EqCannedEmoteCmd_Packet(client->GetVersion());
	if (player->IsAFK()) {
		std::shared_ptr<Spawn> target = client->GetController()->GetTarget();

		packet->spawn_id = client->GetIDForSpawn(player);

		if (target) {
			packet->emote_msg = player->GetName() + " tells " + target->GetName() + " that " + (player->GetGender() == 1 ? "he" : "she") + " is going afk.";
		}
		else {
			packet->emote_msg = player->GetName() + " is going afk.";
		}

		packet->anim_type = 13229;
		packet->unknown = 0;

		// Send to everyone
		zone->chat.SendToPlayersInRange(player, packet, 30.f, true);
	}

	std::string afk_message = player->IsAFK()? "You are now afk." : "You are no longer afk.";
	client->chat.DisplayText("Narrative", afk_message);

	if (player->IsAFK())
	{

		if (sep.GetSize() > 0) {
			//player->SetAwayMessage("I am away from the keyboard, " + string(sep.GetInputString().c_str()));
		}
		else {
			//player->SetAwayMessage("Sorry, I am A.F.K. (Away From Keyboard)");
		}
	}
	else {
		//player->SetAwayMessage("");
	}
}

void CommandProcess::CommandTell(const std::shared_ptr<Client>& client, Separator& sep) {
	// Does not handle cross zones yet only works in same zone
	std::shared_ptr<Entity> player = client->GetController()->GetControlled();
	if (!player) {
		return;
	}

	auto zone = client->GetZone();
	if (!zone) {
		return;
	}

	if (sep.GetSize() < 2) {
		// Send message about not saying anything
		return;
	}

	std::string reciever = sep.GetString(0);
	reciever[0] = toupper(reciever[0]);
	std::string sender = player->GetName();
	std::string message = sep.GetInputString().c_str() + strlen(reciever.c_str()) + 1;


	std::shared_ptr<Entity> reciever_player = zone->GetPlayerEntityByName(reciever.c_str());
	if (!reciever_player) {
		LogDebug(LOG_PLAYER, 0, "CommandTell: too_player was not valid");
		// Send message back to player that recipent was not found
		return;
	}

	if (player == reciever_player) {
		// Send message about not being able to send to yourself
		return;
	}

	// Check if reciever has the sender on Ignore

	std::shared_ptr<Client> reciever_client = zone->GetClientForSpawn(reciever_player);
	if (!reciever_client) {
		LogDebug(LOG_PLAYER, 0, "CommandTell: reciever_client was not valid");
		return;
	}

	HearChatParams params;
	params.bFromNPC = false;
	params.fromSpawnID = 0xFFFFFFFF;
	params.toSpawnID = 0xFFFFFFFF;
	params.fromName = sender.c_str();
	params.toName = reciever.c_str();
	params.bShowBubble = false;
	params.message = message;
	params.chatFilterName = "Tell";

	reciever_client->chat.HearChat(params);
	client->chat.HearChat(params);

	if (reciever_player->IsAFK()) {

		HearChatParams params;
		params.bFromNPC = false;
		params.fromSpawnID = 0xFFFFFFFF;
		params.toSpawnID = 0xFFFFFFFF;
		params.fromName = reciever.c_str();
		params.toName = sender.c_str();
		params.bShowBubble = false;
		params.message = "Temp: Sorry, I am A.F.K. (Away From Keyboard)";
		params.chatFilterName = "Tell";

		reciever_client->chat.HearChat(params);
		client->chat.HearChat(params);
	}
}

void CommandProcess::CommandDepop(const std::shared_ptr<Client>& client, Separator& sep) {
	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone)
		return;

	client->chat.DisplayText("MOTD", "Zone depop starting...", 0xff, false, "");
	zone->Depop();
	client->chat.DisplayText("MOTD", "Zone depop finished!", 0xff, false, "");
}

void CommandProcess::CommandRepop(const std::shared_ptr<Client>& client, Separator& sep) {
	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone)
		return;

	client->chat.DisplayText("MOTD", "Zone depop starting...", 0xff, false, "");
	zone->Depop();
	client->chat.DisplayText("MOTD", "Zone depop finished!", 0xff, false, "");

	client->chat.DisplayText("MOTD", "Zone repop starting...", 0xff, false, "");
	zone->ProcessSpawnLocations();
	client->chat.DisplayText("MOTD", "Zone repop finished!", 0xff, false, "");
}

void CommandProcess::CommandHail(const std::shared_ptr<Client>& client, Separator& sep) {
	auto zone = client->GetZone();
	if (!zone) {
		return;
	}

	auto controller = client->GetController();

	auto player = controller->GetControlled();
	if (!player) {
		return;
	}

	auto target = controller->GetTarget();

	if (!target) {
		zone->chat.HandleSay("Hail", player);
		return;
	}

	//Format the hail message
	ostringstream hailMsg;
	hailMsg << "Hail, " << player->GetName();
	zone->chat.HandleSay(hailMsg.str().c_str(), player);

	//Check if the hailed target is a player, if so we're done. Otherwise try to call the spawn's script
	if (target->IsPlayer()) {
		return;
	}

	target->CallScript("hailed", player);
}