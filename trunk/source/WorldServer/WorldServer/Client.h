#pragma once

#include "../../common/EQ2Stream.h"

class WorldServer;

class Client : public EQ2Stream {
public:
	Client(unsigned int ip, unsigned short port);

	void Process();

	void SetAccount(uint32_t id) { AccountID = id; }
	uint32_t GetAccountID() { return AccountID; }

	void LogIn(std::string user, std::string pass);

	WorldServer* GetServer();

	void SaveErrorsToDB(std::string log, std::string type);

	void SetAllowedRaces(uint32_t val) { AllowedRaces = val; }
	uint32_t GetAllowedRaces();
	void SetAllowedClasses(uint32_t val) { AllowedClasses = val; }
	uint32_t GetAllowedClasses();
	void SetCharacterSlots(uint8_t val) { CharacterSlots = val; }
	uint8_t GetCharacterSlots();

	void SetPendingZone(uint32_t char_id, uint32_t zone_id, uint32_t instance_id);
	uint32_t GetPendingZone() { return pending_zone; }
	uint32_t GetPendingInstance() { return pending_instance; }
	uint32_t GetPendingCharacter() { return pending_character; }

	uint32_t pending_access_code;

protected:
	void ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) override;

private:
	void SendLoginReply(uint8_t reply);

	uint32_t AccountID;
	uint32_t AllowedRaces;
	uint32_t AllowedClasses;
	uint8_t CharacterSlots;

	uint32_t pending_zone;
	uint32_t pending_instance;
	uint32_t pending_character;
};