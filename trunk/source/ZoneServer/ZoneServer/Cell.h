#pragma once

class Spawn;

class Cell {
public:
	Cell(std::pair<int32_t, int32_t> cellCoordinates);
	~Cell();

	void Process();

	std::pair<int32_t, int32_t> GetCellCoordinates() { return m_cellCoordinates; }

private:
	std::pair<int32_t, int32_t> m_cellCoordinates;
	std::vector<std::weak_ptr<Spawn> > m_spawnList;

};