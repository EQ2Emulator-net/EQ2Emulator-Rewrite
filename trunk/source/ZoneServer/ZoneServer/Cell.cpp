#include "stdafx.h"

#include "Cell.h"
#include "../Spawns/Spawn.h"

Cell::Cell(std::pair<int32_t, int32_t> cellCoordinates) {
	m_cellCoordinates = cellCoordinates;
}

Cell::~Cell() {

}

void Cell::Process() {
	std::vector<std::weak_ptr<Spawn> >::iterator itr;
	for (itr = m_spawnList.begin(); itr != m_spawnList.end(); ) {
		std::shared_ptr<Spawn> spawn = (*itr).lock();
		if (spawn) {
			spawn->Process();
			itr++;
		}
		else
			itr = m_spawnList.erase(itr);
	}
}