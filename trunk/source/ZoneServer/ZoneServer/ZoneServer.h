#pragma once

#include <stdint.h>
#include <map>
#include <string>
#include <thread>

class Client;
class Entity;

// This will be where every thing happens
class ZoneServer {
public:
	ZoneServer(uint32_t zone_id);
	~ZoneServer();

	bool Init();

	void Process();

	bool AddClient(std::weak_ptr<Client> client);


	void SendCharacterInfo(std::shared_ptr<Client> client);

private:

	std::vector<std::shared_ptr<Entity> > players;


	std::map<uint32_t, std::weak_ptr<Client> > Clients;
	std::thread process_thread;
	bool isRunning;


	// following is info from `zones` table in the DB
	uint32_t id;
	uint32_t instanceID;
	uint8_t expansionID;
	std::string name;
	std::string file;
	std::string description;
	float safeX;
	float safeY;
	float safeZ;
	float safeHeading;
	float underworld;
	float expModifier;
	uint8_t minRecommended;
	uint8_t maxRecommended;
	std::string zoneType;
	bool alwaysLoaded;
	bool cityZone;
	bool weatherAllowed;
	uint32_t minStatus;
	uint32_t minLevel;
	uint32_t maxLevel;
	uint8_t startZone;
	std::string instanceType;
	uint32_t defaultReenterTime;
	uint32_t defaultResetTime;
	uint32_t defaultLockoutTime;
	uint16_t forceGroupToZone;
	std::string luaScript;
	uint32_t shutdownTimer;
	std::string zoneMOTD;
	uint32_t rulesetID;

public:
	//void SetID(uint32_t val) { id = val; }
	uint32_t GetID() { return id; }
	void SetInstanceID(uint32_t val) { instanceID = val; }
	uint32_t GetInstanceID() { return  instanceID; }
	void SetExpansionID(uint8_t val) { expansionID = val; }
	uint8_t GetExpansionID() { return expansionID; }
	void SetName(std::string val) { name = val; }
	std::string GetName() { return name; }
	void SetFile(std::string val) { file = val; }
	std::string GetFile() { return file; }
	void SetDescription(std::string val) { description = val; }
	std::string GetDescription() { return description; }
	void SetSafeX(float val) { safeX = val; }
	float GetSafeX() { return safeX; }
	void SetSafeY(float val) { safeY = val; }
	float GetSafeY() { return safeY; }
	void SetSafeZ(float val) { safeZ = val; }
	float GetSafeZ() { return safeZ; }
	void SetSafeHeading(float val) { safeHeading = val; }
	float GetSafeHeading() { return safeHeading; }
	void SetUnderworld(float val) { underworld = val; }
	float GetUnderworld() { return underworld; }
	void SetEXPModifier(float val) { expModifier = val; }
	float GetEXPModifier() { return expModifier; }
	void SetMinRecommended(uint8_t val) { minRecommended = val; }
	uint8_t GetMinRecommended() { return minRecommended; }
	void SetMaxRecomended(uint8_t val) { maxRecommended = val; }
	uint8_t GetMaxRecomended() { return maxRecommended; }
	void SetZoneType(std::string val) { zoneType = val; }
	std::string GetZoneType() { return zoneType; }
	void SetIsAlwaysLoaded(bool val) { alwaysLoaded = val; }
	bool GetAlwaysLoaded() { return alwaysLoaded; }
	void SetCityZone(bool val) { cityZone = val; }
	bool GetCityZone() { return cityZone; }
	void SetWeatherAllowed(bool val) { weatherAllowed = val; }
	bool GetWeatherAllowed() { return weatherAllowed; }
	void SetMinStatus(uint32_t val) { minStatus = val; }
	uint32_t GetMinStatus() { return minStatus; }
	void SetMinLevel(uint32_t val) { minLevel = val; }
	uint32_t GetMinLevel() { return minLevel; }
	void SetMaxLevel(uint32_t val) { maxLevel = val; }
	uint32_t GetMaxLevel() { return maxLevel; }
	void SetStartingZone(uint8_t val) { startZone = val; }
	uint8_t GetStartingZone() { return startZone; }
	void SetInstanceType(std::string val) { instanceType = val; }
	std::string GetInstanceType() { return instanceType; }
	void SetDefaultReenterTime(uint32_t val) { defaultReenterTime = val; }
	uint32_t GetDefaultReenterTime() { return defaultReenterTime; }
	void SetDefaultResetTime(uint32_t val) { defaultResetTime = val; }
	uint32_t GetDefaultResetTime() { return defaultResetTime; }
	void SetDefaultLockoutTime(uint32_t val) { defaultLockoutTime = val; }
	uint32_t GetDefaultLockoutTime() { return defaultLockoutTime; }
	void SetForceGroupToZone(uint16_t val) { forceGroupToZone = val; }
	uint16_t GetForceGroupToZone() { return forceGroupToZone; }
	void SetLuaScript(std::string val) { luaScript = val; }
	std::string GetLuaScript() { return luaScript; }
	void SetShutdownTimer(uint32_t val) { shutdownTimer = val; }
	uint32_t GetShutdownTimer() { return shutdownTimer; }
	void SetZoneMOTD(std::string val) { zoneMOTD = val; }
	std::string GetZoneMOTD() { return zoneMOTD; }
	void SetRulesetID(uint32_t val) { rulesetID = val; }
	uint32_t GetRulesetID() { return rulesetID; }

};