/*  
    EQ2Emulator:  Everquest II Server Emulator
    Copyright (C) 2007  EQ2EMulator Development Team (http://www.eq2emulator.net)

    This file is part of EQ2Emulator.

    EQ2Emulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EQ2Emulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EQ2Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "stdafx.h"

#include "RC4.h"

static bool g_bInitStateInitialized = false;
static unsigned char g_byInitState[256];

RC4::RC4(uint64_t nKey)
{
	if( !g_bInitStateInitialized )
	{
		for(uint16_t i = 0; i < 256; i++ )
			g_byInitState[i] = (uint8_t)i;
		g_bInitStateInitialized = true;
	}
	Init(nKey);
}

RC4::~RC4()
{
}

void RC4::Init(uint64_t nKey)
{
	memcpy(m_state, g_byInitState, 256);
	m_x = 0;
	m_y = 0;

	uint32_t dwKeyIndex = 0;
	uint32_t dwStateIndex = 0;
	unsigned char* pKey = (unsigned char*)&nKey;
	for(uint16_t i = 0; i < 256; i++ )
	{
		uint32_t dwTemp = m_state[i];
		dwStateIndex += pKey[dwKeyIndex] + dwTemp;
		dwStateIndex &= 0xFF;
		m_state[i] = m_state[dwStateIndex];
		m_state[dwStateIndex] = (unsigned char)dwTemp;
		dwKeyIndex++;
		dwKeyIndex &= 7;
	}
}

// A = m_state[X + 1]
// B = m_state[Y + A]
// C ^= m_state[(A + B)]

// X = 20
// Y = ?
// C = 0
// m_state[(A + B)] = Cypher Byte

void RC4::Cypher(unsigned char* pBuffer, uint32_t nLength)
{
	uint32_t nOffset = 0;
	unsigned char byKey1 = m_x;
	unsigned char byKey2 = m_y;
	if( nLength > 0 )
	{
		do
		{
			byKey1++;
			unsigned char byKeyVal1 = m_state[byKey1];

			byKey2 += byKeyVal1;
			unsigned char byKeyVal2 = m_state[byKey2];
			
			m_state[byKey1] = byKeyVal2;
			m_state[byKey2] = byKeyVal1;

			pBuffer[nOffset++] ^= m_state[(byKeyVal1 + byKeyVal2) & 0xFF];
		} while( nOffset < nLength );
	}
	m_x = byKey1;
	m_y = byKey2;
}
