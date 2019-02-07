#pragma once

#include "../../common/Server.h"

class WorldServer : public Server {
public:
	WorldServer();

	bool ProcessClientWrite();
	bool ProcessClients();

	void SetName(std::string name) { Name = name; }
	std::string GetName() { return Name; }
	void SetMaxCharactersPerAccount(uint8_t num) { MaxCharactersPerAccount = num; }
	uint8_t GetMaxCharactersPerAccount() { return MaxCharactersPerAccount; }
	void SetAllowedRaces(uint32_t races) { AllowedRaces = races; }
	uint32_t GetAllowedRaces() { return AllowedRaces; }
	void SetAllowedClasses(uint32_t classes) { AllowedClasses = classes; }
	uint32_t GetAllowedClasses() { return AllowedClasses; }

protected:
	Stream* GetNewStream(unsigned int ip, unsigned short port) override;

private:
	std::string Name;
	uint8_t MaxCharactersPerAccount;
	uint32_t AllowedRaces;
	uint32_t AllowedClasses;

};