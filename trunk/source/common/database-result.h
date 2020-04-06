/**
 * @file DatabaseResult.h
 *
 * @brief Database Result functions
 *
 * @details Database Result functions
 *
 * @author Scatman
 * @date 09/05/13
 *
 * @section License
 * VGOEmulator:  Vanguard: Saga of Heroes Server Emulator
 * Copyright (C) 2014  VGOEmulator Development Team (http://vgoemulator.net)
 *
 * This file is part of VGOEmulator.
 *
 * VGOEmulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VGOEmulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VGOEmulator.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMMON_DATABASERESULT_H_
#define COMMON_DATABASERESULT_H_

#include <cstdint>
#include <mysql.h>
#include <deque>

class DatabaseResult {
public:
	DatabaseResult();
	virtual ~DatabaseResult();

	void AddResult(MYSQL_RES* res, bool first);
	bool Next();
	bool NextResultSet();

	bool IsNull(unsigned int index);
	bool IsNullStr(const char* field_name);
	uint8_t GetUInt8(unsigned int index);
	uint8_t GetUInt8Str(const char* field_name);
	int8_t GetInt8(unsigned int index);
	int8_t GetInt8Str(const char* field_name);
	uint16_t GetUInt16(unsigned int index);
	uint16_t GetUInt16Str(const char* field_name);
	int16_t GetInt16(unsigned int index);
	int16_t GetInt16Str(const char* field_name);
	uint32_t GetUInt32(unsigned int index);
	uint32_t GetUInt32Str(const char* field_name);
	int32_t GetInt32(unsigned int index);
	int32_t GetInt32Str(const char* field_name);
	uint64_t GetUInt64(unsigned int index);
	uint64_t GetUInt64Str(const char* field_name);
	int64_t GetInt64(unsigned int index);
	int64_t GetInt64Str(const char* field_name);
	float GetFloat(unsigned int index);
	float GetFloatStr(const char* field_name);
	char GetChar(unsigned int index);
	char GetCharStr(const char* field_name);
	bool GetBool(unsigned int index);
	bool GetBoolStr(const char* field_name);
	const char* GetString(unsigned int index);
	const char* GetStringStr(const char* field_name);

	unsigned int GetNumRows();

private:
	std::deque<MYSQL_RES*> results;
	MYSQL_ROW row;
	MYSQL_RES* current_res;

	const char* GetFieldValue(unsigned int index);
	const char* GetFieldValueStr(const char* field_name);
};

#endif
