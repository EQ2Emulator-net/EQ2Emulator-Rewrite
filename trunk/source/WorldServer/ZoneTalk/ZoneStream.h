#pragma once

#include "../../common/EmuStream.h"
#include <map>
#include <set>
#include <vector>

class ZoneTalk;

struct PlayerInfo {
	uint32_t account_id;
	uint32_t character_id;
	std::string character_name;
	std::string zone_name;
	uint8_t level;
	uint8_t class_;
};

class ZoneStream : public EmuStream, public std::enable_shared_from_this<ZoneStream> {
public:
	ZoneStream(uint32_t ip, uint32_t port);
	~ZoneStream() = default;

	void Process() override;
	ZoneTalk* GetServer();

	uint32_t GetNumberOfZones();
	void AddZone(uint32_t zone_id, uint32_t instance_id);
	bool HasZone(uint32_t zone_id, uint32_t instance_id = 0);

private:
	std::string ip;
	uint16_t port;
	uint16_t max_zones;
	uint32_t num_players;

	// key = zone_id, set is instance ids
	std::map<uint32_t, std::set<uint32_t> > current_zones;

	std::vector<PlayerInfo> current_clients;

public:
	void SetIP(std::string val) { ip = val; }
	std::string GetIP() { return ip; }
	void SetPort(uint16_t val) { port = val; }
	uint16_t GetPort() { return port; }
	void SetMaxZones(uint16_t val) { max_zones = val; }
	uint16_t GetMaxZones() { return max_zones; }
	void SetNumberOfPlayers(uint32_t val) { num_players = val; }
	uint32_t GetNumberOfPlayers() { return num_players; }
};