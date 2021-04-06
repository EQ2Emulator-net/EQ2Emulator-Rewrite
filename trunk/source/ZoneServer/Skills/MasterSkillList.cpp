#include "stdafx.h"

#include "MasterSkillList.h"
#include "../Packets/OP_PopulateSkillMapsMsg.h"
#include "../Database/ZoneDatabase.h"
#include "../../common/log.h"
#include "../ZoneServer/Client.h"

void MasterSkillList::AssignSkills(std::unordered_map<uint32_t, std::shared_ptr<const Skill>>& s) {
	WriteLocker l(m_skills);
	skills = std::move(s);
}

void ZoneDatabase::LoadMasterSkillList(MasterSkillList& sl) {
	DatabaseResult res;
	if (!Select(&res, "SELECT * FROM `skills`;")) {
		LogError(LOG_DATABASE, 0, "Error loading the master skill list.");
		return;
	}

	std::unordered_map<uint32_t, std::shared_ptr<const Skill>> skills;
	skills.reserve(res.GetNumRows());

	while (res.Next()) {
		auto skill = std::make_shared<Skill>();
		skill->skillID = res.GetUInt32(0);
		skill->shortName = res.GetString(1);
		skill->name = res.GetString(2);
		skill->description = res.GetString(3);
		skill->skillType = res.GetUInt32(4);
		skill->bDisplay = res.GetBool(5);
		skills.emplace(skill->skillID, skill);
	}

	LogDebug(LOG_DATABASE, 0, "Successfully loaded %u skills.", static_cast<uint32_t>(skills.size()));

	sl.AssignSkills(skills);
}

void MasterSkillList::SendSkillMapPacket(const std::shared_ptr<Client>& client) {
	OP_PopulateSkillMapsMsg_Packet p(client->GetVersion());
	
	ReadLocker l(m_skills);
	p.skills.resize(skills.size());

	size_t i = 0;
	for (auto& itr : skills) {
		auto& s = p.skills[i++];
		std::shared_ptr<const Skill>& ref = itr.second;
		s.skillID = ref->skillID;
		s.name = ref->name;
		s.shortName = ref->shortName;
		s.description = ref->description;
	}

	client->QueuePacket(p);
}