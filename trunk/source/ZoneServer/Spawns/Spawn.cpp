#include "stdafx.h"

#include "Spawn.h"

Spawn::Spawn() {
	memset(&m_posStruct, 0, sizeof(m_posStruct));
	memset(&m_infoStruct, 0, sizeof(m_infoStruct));
	memset(&m_visStruct, 0, sizeof(m_visStruct));
	memset(&m_titleStruct, 0, sizeof(m_titleStruct));
}

Spawn::~Spawn() {

}