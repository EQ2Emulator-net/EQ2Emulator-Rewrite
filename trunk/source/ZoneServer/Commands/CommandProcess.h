#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <map>
#include "CommandToken.h"

class Separator;
class Client;

class CommandProcess {
public:
	CommandProcess();
	~CommandProcess() = default;

	void AddCommand(uint32_t handler_id, const std::string& cmd, uint32_t parent_id, const std::string& required_status);
	void ProcessCommand(const std::shared_ptr<Client>& client, uint32_t handler, const std::string& args);
	void SendCommandList(const std::shared_ptr<Client>& client);

private:
	//Handler Registration functions
	static void CommandHandlerPrototype(const std::shared_ptr<Client>& client, Separator& sep);
	typedef decltype(&CommandHandlerPrototype) CommandHandler_t;
	void RegisterCommands();
	void RegisterCommandHandler(uint32_t handler_id, CommandHandler_t handler);
	std::map<uint32_t, CommandHandler_t> handlers;

	struct CommandInfo {
		CommandInfo() : handler(nullptr) {}
		CommandHandler_t handler;
		CommandToken token;
		std::string cmdText;
		//map<subCommandName, CommandInfo>
		std::map<std::string, std::shared_ptr<CommandInfo> > subCommands;
	};

	//map<handler_client_index, CommandInfo>
	std::map<uint32_t, std::shared_ptr<CommandInfo> > rootCommands;
	//map<handler_id, CommandInfo>
	std::map<uint32_t, std::shared_ptr<CommandInfo> > commandsParentLookup;

	//Handlers
	static void CommandSpeed(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandMove(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandTest(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandZone(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandFlymode(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSay(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandShout(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandEmote(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandOOC(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandGrid(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandAFK(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandTell(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandDepop(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandRepop(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandHail(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandDevMode(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSetEmoteVoice(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSetCombatVoice(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandGoto(const std::shared_ptr<Client>& client, Separator& sep);

	// CommandSpawn.cpp
	static void CommandSpawnSet(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSpawnDetails(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSpawn(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandPath(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSpawnCamp(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSpawnRemove(const std::shared_ptr<Client>& client, Separator& sep);
	static void CommandSpawnSet_EyeColor(const std::shared_ptr<Client>& client, Separator& sep);
};