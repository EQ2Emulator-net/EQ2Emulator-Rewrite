#pragma once

#include "Spawn.h"

class Object : public Spawn {
public:
	Object() {
		m_objectDatabaseID = 0;
		m_deviceID = 0;
		SetState(1, false);
	}

	Object(std::shared_ptr<Object> in) : Spawn(in) {
		m_objectDatabaseID = in->GetObjectDatabaseID();
		m_deviceID = in->GetDeviceID();
		SetState(in->GetPosStruct()->positionState, false);
	}

	bool IsObject() override { return true; }
	void SetObjectDatabaseID(uint32_t id) { m_objectDatabaseID = id; }
	uint32_t GetObjectDatabaseID() { return m_objectDatabaseID; }
	void SetDeviceID(uint8_t id) { m_deviceID = id; }
	uint8_t GetDeviceID() { return m_deviceID; }

private:
	uint32_t m_objectDatabaseID;
	uint8_t m_deviceID;

};