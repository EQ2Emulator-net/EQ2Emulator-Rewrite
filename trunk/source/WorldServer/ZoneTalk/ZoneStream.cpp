#include "stdafx.h"

#include "ZoneStream.h"
#include "../../common/util.h"
#include "../../common/EmuPacket.h"
#include "ZoneTalk.h"

void ZoneStream::Process() {
	auto incoming = PopIncoming();

	for (auto& itr : incoming) {
		LogDebug(LOG_PACKET, 0, "ZoneStream::incoming dump");
		DumpBytes(itr->buffer, itr->Size);
		itr->HandlePacket(shared_from_this());
	}

	auto outgoing = PopOutgoing();

	for (auto& itr : outgoing) {
		WritePacket(Sock, itr->buffer, itr->Size);
	}
}

ZoneTalk* ZoneStream::GetServer() {
	return static_cast<ZoneTalk*>(server);
}

uint32_t ZoneStream::GetNumberOfZones() {
	uint32_t ret = 0;

	for (std::pair<uint32_t, std::set<uint32_t> > kvp : current_zones) {
		ret += kvp.second.size();
	}

	return ret;
}

void ZoneStream::AddZone(uint32_t zone_id, uint32_t instance_id) {
	current_zones[zone_id].insert(instance_id);
}

bool ZoneStream::HasZone(uint32_t zone_id, uint32_t instance_id) {
	bool ret = false;

	std::map<uint32_t, std::set<uint32_t> >::iterator itr = current_zones.find(zone_id);
	if (itr != current_zones.end()) {
		if (instance_id == 0)
			ret = true;
		else {
			ret = itr->second.count(instance_id) > 0;
		}
	}

	return ret;
}