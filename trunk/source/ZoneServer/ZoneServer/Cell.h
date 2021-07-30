#pragma once

class Spawn;
class Client;

class Cell {
public:
	Cell(std::pair<int32_t, int32_t> cellCoordinates);
	~Cell();

	void Process();

	std::pair<int32_t, int32_t> GetCellCoordinates() { return m_cellCoordinates; }

	void AddSpawn(std::shared_ptr<Spawn> spawn, bool force = false);
	void RemoveSpawn(std::shared_ptr<Spawn> spawn, bool force = false);
	bool IsActive() { return bIsActive; }
	void ActivateCell(std::shared_ptr<Client> client);
	void SendRemoveSpawnsForClient(std::shared_ptr<Client> client);
	void DeactivateCell();
	void ClearSpawnList();

private:
	std::pair<int32_t, int32_t> m_cellCoordinates;
	std::vector<std::weak_ptr<Spawn> > m_spawnList;
	std::vector<std::shared_ptr<Spawn> > m_pendingRemoveSpawnList;
	std::vector<std::shared_ptr<Spawn> > m_pendingAddSpawnList;
	bool bIsActive;

};