#pragma once

#include "Skill.h"
#include "../../common/Mutex.h"

#include <unordered_map>
#include <memory>

class MasterSkillList {
public:
	MasterSkillList() : m_skills("MasterSkillList::m_skills") {}
	~MasterSkillList() = default;

	void AssignSkills(std::unordered_map<uint32_t, std::shared_ptr<const Skill>>& s);
	void SendSkillMapPacket(const std::shared_ptr<class Client>& client);

private:
	std::unordered_map<uint32_t, std::shared_ptr<const Skill>> skills;
	Mutex m_skills;
};