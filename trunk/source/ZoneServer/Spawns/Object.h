#pragma once

#include "Spawn.h"

class Object : public Spawn {
public:

	void SetObjectDatabaseID(uint32_t id) { m_objectDatabaseID = id; }
	uint32_t GetObjectDatabaseID() { return m_objectDatabaseID; }
	void SetDeviceID(uint8_t id) { m_deviceID = id; }
	uint8_t GetDeviceID() { return m_deviceID; }

private:
	uint32_t m_objectDatabaseID;
	uint8_t m_deviceID;

};