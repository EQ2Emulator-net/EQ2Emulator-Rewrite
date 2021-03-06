#pragma once

#include <cstdint>
#include <mysql.h>
#include <deque>

class DatabaseResult {
public:
	DatabaseResult();
	virtual ~DatabaseResult();

	void Clear();

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
