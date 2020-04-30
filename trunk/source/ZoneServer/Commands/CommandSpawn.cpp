#include "stdafx.h"

#include"CommandProcess.h"
#include "../ZoneServer/Client.h"
#include "../../common/Separator.h"
#include "../Database/ZoneDatabase.h"
#include "../Spawns/Spawn.h"
#include "../Controllers/PlayerController.h"

void CommandProcess::CommandSpawnSet(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() < 2) {
		//syntax error
		return;
	}

	std::shared_ptr<PlayerController> controller = client->GetController();
	if (!controller)
		return;

	std::shared_ptr<Spawn> target = controller->GetTarget();
	if (!target)
		return;

	// Title struct
	std::string cmd = sep.GetString(0);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	if (cmd == "name") {
		target->SetName(sep.GetString(1));
	}

	if (cmd == "lastname") {
		target->SetLastName(sep.GetString(1));
	}

	if (cmd == "suffixtitle") {
		target->SetSuffixTitle(sep.GetString(1));
	}

	if (cmd == "prefixtitle") {
		target->SetPrefixTitle(sep.GetString(1));
	}

	if (cmd == "pvptitle") {
		target->SetPvPTitle(sep.GetString(1));
	}

	if (cmd == "guild") {
		target->SetGuild(sep.GetString(1));
	}

	// Info struct
	if (cmd == "visual_state") {
		target->SetVisualState(sep.GetUInt32(1));
	}

	// Vis struct

	// Pos struct
}