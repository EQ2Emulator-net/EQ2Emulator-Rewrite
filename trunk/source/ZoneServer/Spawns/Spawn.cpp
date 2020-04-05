#include "stdafx.h"

#include "Spawn.h"

Spawn::Spawn() : m_updateFlagsByte(0) {
	memset(&m_posStruct, 0, sizeof(m_posStruct));
	memset(&m_infoStruct, 0, sizeof(m_infoStruct));
	memset(&m_visStruct, 0, sizeof(m_visStruct));
}

Spawn::~Spawn() {

}

Spawn::UpdateFlags Spawn::PopUpdateFlags() {
	UpdateFlags ret = m_updateFlags;
	m_updateFlagsByte = 0;
	return ret;
}