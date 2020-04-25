#pragma once

#include "../../common/UDPServer.h"

#include "../../common/Packets/EQ2Packets/OP_LoginReplyMsg_Packet.h"

class Character;
class ZoneStream;

class WorldServer : public UDPServer {
public:
	WorldServer();

	bool Process() override;
	bool ProcessClientWrite();
	bool ProcessClients();

	void SetID(uint32_t id) override { ID = id; }
	uint32_t GetID() { return ID; }
	void SetName(std::string name) { Name = name; }
	std::string GetName() { return Name; }
	void SetLocked(bool val) { Locked = val; }
	bool GetLocked() { return Locked; }
	void SetCharactersSlotsPerAccount(uint8_t num) { CharacterSlotsPerAccount = num; }
	uint8_t GetCharacterSlotsPerAccount() { return CharacterSlotsPerAccount; }
	void SetMaxAdvLevel(uint8_t level) { MaxAdvLevel = level; }
	uint8_t GetMaxAdvLevel() { return MaxAdvLevel; }
	void SetMaxTSLevel(uint8_t level) { MaxTSLevel = level; }
	uint8_t GetMaxTSLevel() { return MaxTSLevel; }
	void SetAllowedRaces(uint32_t races) { AllowedRaces = races; }
	uint32_t GetAllowedRaces() { return AllowedRaces; }
	void SetAllowedClasses(uint32_t classes) { AllowedClasses = classes; }
	uint32_t GetAllowedClasses() { return AllowedClasses; }
	void SetAutoAccountCreation(bool val) { AutoAccountCreation = val; }
	bool GetAutoAccountCreation() { return AutoAccountCreation; }

	void AddPendingClientTransfer(uint32_t access_code, const std::shared_ptr<Character>& character, const std::shared_ptr<ZoneStream>& stream);
	void ConfirmClientTransfer(uint32_t access_code, uint32_t sessionID, const std::string& zoneName);

	std::shared_ptr<class Client> GetClientByAccessCode(uint32_t code);

	std::map<uint8_t, std::vector<OP_LoginReplyMsg_Packet::ClassItem::StartingItem> > NormalEquipment;
	std::map<uint8_t, std::vector<OP_LoginReplyMsg_Packet::ClassItem::StartingItem> > LVL90Equipment;
	std::map<uint8_t, std::vector<OP_LoginReplyMsg_Packet::ClassItem::StartingItem> > TLEquipment;
protected:
	std::shared_ptr<Stream> GetNewStream(unsigned int ip, unsigned short port) override;

private:
	uint32_t ID;
	std::string Name;
	bool Locked;
	uint8_t CharacterSlotsPerAccount;
	uint8_t MaxAdvLevel;
	uint8_t MaxTSLevel;
	uint32_t AllowedRaces;
	uint32_t AllowedClasses;
	bool AutoAccountCreation;

	struct PendingClientTransfer {
		uint32_t timeout;
		std::weak_ptr<Character> character;
		std::weak_ptr<ZoneStream> stream;
	};

	std::map<uint32_t, PendingClientTransfer> pendingClientTransfers;
};