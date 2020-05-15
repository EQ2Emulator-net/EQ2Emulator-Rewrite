#include "stdafx.h"

#include "Spawn.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/BaseController.h"
#include "../Controllers/PlayerController.h"
#include "../../common/timer.h"
#include "Entity.h"
#include "../ZoneServer/ZoneServer.h"
// Packets
#include "../Packets/OP_UpdateSpawnCmdMsg.h"
#include "../Packets/OP_UpdateTitleCmd_Packet.h"
#include "../Lua/LuaGlobals.h"
#include "../Lua/LuaInterface.h"

extern LuaGlobals g_luaGlobals;

Spawn::Spawn() : m_updateFlagsByte(0) {
	m_spawnID = GetNextID();
	m_spawnDatabaseID = 0;
	movementTimestamp = Timer::GetServerTime();
	m_sizeOffset = 0.0f;
	bShowName = true;
	bAttackable = false;
	visUpdateTag = 0;
	lastVisUpdateSent = 0;
	//Targetable by default
	bShowLevel = true;
	bShowCommandIcon = true;
	m_scriptID = 0;
	m_minLevel = 0;
	m_maxLevel = 0;
	m_encounterOffset = 0;
	m_factionID = 0;
	m_hp = 0;
	m_power = 0;
	m_savagery = 0;
	m_dissonance = 0;
	m_merchantID = 0;
	m_merchantType = 0;
	m_lastFaceTargetTime = 0;
	m_globalSpawn = false;
	m_bFaceOnHail = false;
	m_bDisableLoot = false;
	m_spawnLocationID = 0;
	m_spawnEntryID = 0;
	m_groupID = 0;
	m_respawnTime = 0;
	m_expireTime = 0;
	m_spawnLocationPlacementID = 0;
	m_origX = 0.0f;
	m_origY = 0.0f;
	m_origZ = 0.0f;
	m_origHeading = 0.0f;
	m_origPitch = 0.0f;
	m_origRoll = 0.0f;
	m_primaryCommandListID = 0;
	m_secondaryCommandListID = 0;
}

Spawn::Spawn(std::shared_ptr<Spawn> in) {
	memcpy(&m_infoStruct, in->GetInfoStruct(), sizeof(m_infoStruct));
	memcpy(&m_posStruct, in->GetPosStruct(), sizeof(m_posStruct));
	m_spawnID = GetNextID();
	movementTimestamp = Timer::GetServerTime();
	m_titleStruct = *in->GetTitleStruct();
	m_updateFlagsByte = 0;
	if (in->GetSignData() != nullptr) {
		signData = std::make_unique<Sign>(*in->GetSignData());
		m_posStruct.swimmingSpeedMultiplier = 0.34375f;
	}
	if (in->GetWidgetData() != nullptr) {
		widgetData = std::make_unique<Widget>(*in->GetWidgetData());
		m_posStruct.swimmingSpeedMultiplier = 0.34375f;
	}
	m_spawnID = GetNextID();
	m_spawnDatabaseID = in->GetDatabaseID();
	m_sizeOffset = in->GetSizeOffset();
	m_primaryCommandListID = in->GetPrimaryCommandListID();
	m_secondaryCommandListID = in->GetSecondaryCommandListID();
	m_factionID = in->GetFactionID();
	m_hp = in->GetHP();
	m_power = in->GetPower();
	m_savagery = in->GetSavagery();
	m_dissonance = in->GetDissonance();
	m_merchantID = in->GetMerchantID();
	m_merchantType = in->GetMerchantType();
	m_spawnLocationID = 0;
	m_spawnEntryID = 0;
	m_groupID = 0;
	m_respawnTime = 0;
	m_expireTime = 0;
	m_spawnLocationPlacementID = 0;
	m_origX = 0.0f;
	m_origY = 0.0f;
	m_origZ = 0.0f;
	m_origHeading = 0.0f;
	m_origPitch = 0.0f;
	m_origRoll = 0.0f;
	m_posStruct.movementMode = 2;
	m_lastFaceTargetTime = 0;

	m_minLevel = in->GetMinLevel();
	m_maxLevel = in->GetMaxLevel();
	m_encounterOffset = in->GetDifficultyOffset();

	if (m_posStruct.collisionRadius == 0.0f)
		m_posStruct.collisionRadius = 1.0f;

	if (m_posStruct.sizeRatio == 0.0f)
		m_posStruct.sizeRatio = 1.0f;

	if (m_posStruct.size == 0.0f)
		m_posStruct.size = 1.0f;
	
	if (m_sizeOffset > 0.0f)
		m_posStruct.size = MakeRandom(m_posStruct.size, m_posStruct.size + m_sizeOffset);

	if (m_maxLevel > m_minLevel) {
		m_infoStruct.level = static_cast<uint8_t>(MakeRandomInt(m_minLevel, m_maxLevel));
		SetOrigLevel(m_infoStruct.level, false);
	}

	if (m_encounterOffset > 0)
		m_infoStruct.difficulty = static_cast<uint8_t>(MakeRandomInt(m_infoStruct.difficulty, m_infoStruct.difficulty + m_encounterOffset));

	bAttackable = in->bAttackable;
	bShowName = in->bShowName;
	bShowCommandIcon = in->bShowCommandIcon;
	bShowLevel = in->bShowLevel;
	visUpdateTag = 0;
	lastVisUpdateSent = 0;

	m_primaryCommandList = in->m_primaryCommandList;
	m_secondaryCommandList = in->m_secondaryCommandList;
	m_scriptID = in->m_scriptID;
	m_globalSpawn = false;
	m_bFaceOnHail = in->m_bFaceOnHail;
	m_bDisableLoot = in->m_bDisableLoot;
}

Spawn::~Spawn() {

}

void Spawn::Process() {
	// m_controller->Process();

	//TODO: move to controller and add a rule for the amount of time?
	if (m_lastFaceTargetTime != 0 && Timer::GetServerTime() - m_lastFaceTargetTime >= 20000) {
		SetHeading(GetOrigHeading());
		m_lastFaceTargetTime = 0;
	}

	bool bCheckVisUpdate;
	if (bCheckVisUpdate = visUpdateTag != lastVisUpdateSent) {
		//This spawn may have changed their vis struct with clients, so check this loop
		lastVisUpdateSent = visUpdateTag;
	}

	UpdateFlags spawnUpdateFlags = PopUpdateFlags();

	if (m_clients.empty()) {
		return;
	}

	//Using optional so we can have this on the stack while not automatically constructing it
	std::optional<OP_UpdateGhostCmdMsg_Packet> packet;

	for (auto& itr : m_clients) {
		//We may need to modify these flags for this spawn
		union {
			UpdateFlags update;
			uint8_t updateByte;
		};
		update = spawnUpdateFlags;

		bool bCheckVis = bCheckVisUpdate;
		uint32_t& visTag = itr.second.second;
		uint32_t& visCRC = itr.second.first;
		std::shared_ptr<Client> client = itr.first.lock();

		if (!client) {
			continue;
		}

		auto player = client->GetController()->GetControlled();

		//Check if the client's player needs a vis re-check
		if (player && player->GetVisUpdateTag() != visTag) {
			visTag = player->GetVisUpdateTag();
			bCheckVis = true;
		}

		// Don't send position updates to yourself
		if (player.get() == this) {
			update.m_posChanged = false;
		}

		//We definitely have no updates to send this round
		if (updateByte == 0 && !bCheckVis) {
			continue;
		}

		if (update.m_titleChanged) {
			OP_UpdateTitleCmd_Packet p2(client->GetVersion());
			p2.spawn_id = client->GetIDForSpawn(shared_from_this());

			p2.name = m_titleStruct.name;
			p2.unknown1 = m_titleStruct.unknown1;
			p2.isPlayer = m_titleStruct.isPlayer;
			p2.last_name = m_titleStruct.last_name;
			p2.suffix_title = m_titleStruct.suffix_title;
			p2.prefix_title = m_titleStruct.prefix_title;
			p2.pvp_title = m_titleStruct.pvp_title;
			p2.guild_title = m_titleStruct.guild;

			client->QueuePacket(p2);
		}

		uint16_t index = client->GetIndexForSpawn(shared_from_this());
		
		//Check vis first because we may not actually need to send an update for this client
		if (bCheckVis) {
			SpawnVisualizationStruct vis;
			vis.DetermineForClient(client, shared_from_this());
			uint32_t newCRC = vis.CalculateCRC();

			if (visCRC != newCRC) {
				//We have an update to send!
				visCRC = newCRC;
				if (!packet)
					packet.emplace(client->GetVersion());
				else if (packet->GetVersion() != client->GetVersion())
					packet->ResetVersion(client->GetVersion());
				static_cast<SpawnVisualizationStruct&>(packet->vis.data) = vis;
				packet->vis.spawnIndex = index;
			}
			else if (!update.m_infoChanged && !update.m_posChanged) {
				continue;
			}
		}
		else if (!update.m_infoChanged && !update.m_posChanged) {
			continue;
		}
		else if (packet) {
			packet->vis.spawnIndex = 0;
		}
		

		if (!packet)
			packet.emplace(client->GetVersion());
		else if (packet->GetVersion() != client->GetVersion())
			packet->ResetVersion(client->GetVersion());

		packet->timestamp = Timer::GetServerTime();

		if (update.m_infoChanged)
			packet->InsertSpawnInfoData(*GetInfoStruct(), index);
		else
			packet->info.spawnIndex = 0;

		if (update.m_posChanged && client->GetController()->GetControlled().get() != this)
			packet->InsertSpawnPosData(client, shared_from_this(), index, true, movementTimestamp);
		else
			packet->pos.spawnIndex = 0;

		packet->SetEncodedBuffers(client, index);
		client->QueuePacket(*packet);
	}
}

void Spawn::AddClient(std::weak_ptr<Client> client, uint32_t visCRC, uint32_t visUpdateTag) {
	m_clients[client] = std::make_pair(visCRC, visUpdateTag);
}

void Spawn::RemoveClient(std::weak_ptr<Client> client) {
	m_clients.erase(client);
}

Spawn::UpdateFlags Spawn::PopUpdateFlags() {
	UpdateFlags ret = m_updateFlags;
	m_updateFlagsByte = 0;
	return ret;
}

const SpawnPositionStruct* Spawn::GetPosStruct() const {
	return &m_posStruct;
}

const SpawnInfoStruct* Spawn::GetInfoStruct() const {
	return &m_infoStruct;
}

const SpawnTitleStruct* Spawn::GetTitleStruct() const {
	return &m_titleStruct;
}

uint32_t Spawn::GetNextID() {
	static std::atomic<uint32_t> g_spawnID(1);
	uint32_t ret;
	//There was a comment in old code about the ID ending in 255 crashing the client, not sure if it's true but doing it anyway
	while (ret = g_spawnID.fetch_add(1), (ret & 0xFF) == 0xFF);
	return ret;
}

void Spawn::UpdateCellCoordinates() {
	std::shared_ptr<ZoneServer> zone = m_zone.lock();
	if (!zone)
		return;

	std::pair<int32_t, int32_t> oldCell = m_currentCellCoordinates;
	m_currentCellCoordinates = zone->GetCellCoordinatesForSpawn(shared_from_this());
	if (m_currentCellCoordinates != oldCell)
		zone->ChangeSpawnCell(shared_from_this(), oldCell);
}

float Spawn::GetDistance(float x1, float y1, float z1, float x2, float y2, float z2, bool ignore_y) {
	x1 -= x2;
	z1 -= z2;

	x1 *= x1;
	z1 *= z1;

	if (ignore_y) {
		y1 = 0.f;
	}
	else {
		y1 -= y2;
		y1 *= y1;
	}

	return std::sqrt(x1 + y1 + z1);
}

float Spawn::GetDistance(float x, float y, float z, bool ignore_y) {
	return GetDistance(x, y, z, GetX(), GetY(), GetZ(), ignore_y);
}

float Spawn::GetDistance(const std::shared_ptr<Spawn>& spawn, bool ignore_y) {
	return GetDistance(spawn->GetX(), spawn->GetY(), spawn->GetZ(), ignore_y);
}

void Spawn::SetAppearanceEquipmentItem(uint8_t slotID, const EQ2EquipmentItem& item, bool updateFlags) {
	SetInfo(&m_infoStruct.equipment_colors[slotID], item.color, false);
	SetInfo(&m_infoStruct.equipment_highlights[slotID], item.highlight, false);
	SetInfo(&m_infoStruct.equipment_types[slotID], item.type, updateFlags);
}

EConsiderDifficulty Spawn::GetConsiderDifficulty(uint16_t considererLevel, uint16_t targetLevel) {
	EConsiderDifficulty color;
	int16_t diff = targetLevel - considererLevel;
	if (considererLevel < 10)
		diff *= 3;
	else if (considererLevel <= 20)
		diff *= 2;
	if (diff >= 9)
		color = EConsiderDifficulty::RED;
	else if (diff >= 5)
		color = EConsiderDifficulty::ORANGE;
	else if (diff >= 1)
		color = EConsiderDifficulty::YELLOW;
	else if (diff == 0)
		color = EConsiderDifficulty::WHITE;
	else if (diff <= -11)
		color = EConsiderDifficulty::GRAY;
	else if (diff <= -6)
		color = EConsiderDifficulty::GREEN;
	else //if(diff < 0)
		color = EConsiderDifficulty::BLUE;
	return color;
}

void Spawn::CallScript(const char* function, const std::shared_ptr<Spawn>& spawnArg, const char* stringArg) {
	auto script = m_luaState.lock();
	if (!script) {
		if (m_scriptID != 0) {
			auto zone = GetZone();
			if (zone) {
				script = zone->LoadSpawnState(m_scriptID);
			}
		}

		if (!script) {
			return;
		}

		//Cache this script for later
		m_luaState = script;
	}

	std::shared_ptr<EmuLuaState> recursiveState;
	lua_State* state = script->state;

	lua_getglobal(state, function);

	if (!lua_isfunction(state, lua_gettop(state))) {
		//This function does not exist in the script, we don't need to spam an error here just return
		lua_pop(state, 1);
		return;
	}

	//NOTE: If we ever have more than one thread that could access this at once we'll need a recursive mutex on the EmuLuaState
	if (script->nUsers++) {
		lua_pop(state, 1);

		//We're calling this script recursively. Load a new copy of the state
		recursiveState = LuaInterface::LoadSpawnScript(m_scriptID);
		if (!recursiveState) {
			--script->nUsers;
			return;
		}

		state = recursiveState->state;
		lua_getglobal(state, function);
	}

	LuaInterface::PushLuaSpawn(state, shared_from_this());
	int32_t nArgs = 1;
	if (spawnArg) {
		LuaInterface::PushLuaSpawn(state, spawnArg);
		++nArgs;
	}

	if (stringArg) {
		LuaInterface::PushLuaString(state, stringArg);
		++nArgs;
	}

	if (lua_pcall(state, nArgs, 0, 0) != LUA_OK) {
		LuaInterface::PrintStateError(state);
	}

	lua_settop(state, 0);

	--script->nUsers;
}

void Spawn::FaceSpawn(const std::shared_ptr<Spawn>& spawn) {
	FaceLocation(spawn->GetX(), spawn->GetZ());
}

//Taken from old code
void Spawn::FaceLocation(float x, float z) {
	double angle;

	double diff_x = x - GetX();
	double diff_z = z - GetZ();

	//If we're very close to the same spot don't bother changing heading
	if (sqrt(diff_x * diff_x * diff_z * diff_z) < .1) {
		return;
	}

	if (diff_z == 0) {
		if (diff_x > 0)
			angle = 90;
		else
			angle = 270;
	}
	else
		angle = ((std::atan(diff_x / diff_z)) * 180) / 3.14159265358979323846;

	if (angle < 0)
		angle = angle + 360;
	else
		angle = angle + 180;

	if (diff_x < 0)
		angle = angle + 180;

	SetHeading(static_cast<float>(angle));

	if (!IsInCombat()) {
		//TODO: Should we add a temporary action state?
		m_lastFaceTargetTime = Timer::GetServerTime();
	}
}

bool Spawn::EntityCommandPrecheck(const std::string& command, const std::shared_ptr<Client>& client) {
	auto script = m_luaState.lock();
	if (!script) {
		if (m_scriptID != 0) {
			auto zone = GetZone();
			if (zone) {
				script = zone->LoadSpawnState(m_scriptID);
			}
		}

		if (!script) {
			return true;
		}

		//Cache this script for later
		m_luaState = script;
	}

	auto player = client->GetController()->GetControlled();
	if (!player) {
		return true;
	}

	std::shared_ptr<EmuLuaState> recursiveState;
	lua_State* state = script->state;

	lua_getglobal(state, "can_use_command");

	if (!lua_isfunction(state, lua_gettop(state))) {
		//This function does not exist in the script, we don't need to spam an error here just return
		lua_pop(state, 1);
		return true;
	}

	//NOTE: If we ever have more than one thread that could access this at once we'll need a recursive mutex on the EmuLuaState
	if (script->nUsers++) {
		lua_pop(state, 1);

		//We're calling this script recursively. Load a new copy of the state
		recursiveState = LuaInterface::LoadSpawnScript(m_scriptID);
		if (!recursiveState) {
			--script->nUsers;
			return true;
		}

		state = recursiveState->state;
		lua_getglobal(state, "can_use_command");
	}

	LuaInterface::PushLuaSpawn(state, shared_from_this());
	LuaInterface::PushLuaSpawn(state, player);
	LuaInterface::PushLuaString(state, command.c_str());

	bool ret = true;

	if (lua_pcall(state, 3, 1, 0) != LUA_OK) {
		LuaInterface::PrintStateError(state);
	}
	else {
		ret = LuaInterface::GetLuaBool(state, lua_gettop(state));
	}

	lua_settop(state, 0);

	--script->nUsers;

	return ret;
}

void Spawn::HandleEntityCommand(const std::string& command, const std::shared_ptr<Client>& client) {
	auto player = client->GetController()->GetControlled();
	if (!player) {
		return;
	}

	if (!EntityCommandPrecheck(command, client)) {
		return;
	}

	//TODO: Add more commands and add cast timers when we have spells
	if (command == "hail") {
		player->Hail(shared_from_this());
	}
	else if (command == "frommerchant") {
		//This will also open the merchant window later
		player->Hail(shared_from_this());
	}
	else if (command == "bank") {
		//This will also open the banker window later
		player->Hail(shared_from_this());
	}
	else if (command == "repair") {
		//This will also open the mender window later
		player->Hail(shared_from_this());
	}

	//I know "casted_on" isn't proper english but that is what was used in old code
	CallScript("casted_on", player, command.c_str());
}

void Spawn::Hail(const std::shared_ptr<Spawn>& target) {
	auto zone = GetZone();
	if (!zone) {
		return;
	}

	auto _this = shared_from_this();

	if (!target) {
		zone->chat.HandleSay("Hail", _this);
		return;
	}

	//Format the hail message
	ostringstream hailMsg;
	hailMsg << "Hail, " << target->GetName();
	zone->chat.HandleSay(hailMsg.str().c_str(), _this);

	//Check if the hailed target is a player, if so we're done. Otherwise try to call the spawn's script
	if (target->IsPlayer()) {
		return;
	}

	target->CallScript("hailed", _this);

	if (target != _this && target->ShouldFaceOnHail()) {
		target->FaceSpawn(_this);
	}
}