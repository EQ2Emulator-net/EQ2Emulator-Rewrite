#include "stdafx.h"

#include "log.h"
#include "database-result.h"

 //return this instead of NULL for certain functions to prevent crashes from coding errors
static const char* empty_str = "";

DatabaseResult::DatabaseResult() :
	current_res(nullptr),
	row(nullptr)
{
}

DatabaseResult::~DatabaseResult() {
	for (auto& itr : results) {
		mysql_free_result(itr);
	}
	if (current_res) {
		mysql_free_result(current_res);
	}
}

void DatabaseResult::AddResult(MYSQL_RES* res, bool first) {
	if (first) {
		if (!results.empty()) {
			for (auto& itr : results) {
				mysql_free_result(itr);
			}
			results.clear();
		}
		if (current_res) {
			mysql_free_result(current_res);
		}
		current_res = res;
	}
	else {
		results.push_back(res);
	}
}

const char* DatabaseResult::GetFieldValue(unsigned int index) {
	if (!current_res) {
		return NULL;
	}

	unsigned int num_fields = mysql_num_fields(current_res);
	if (index >= num_fields) {
		LogError(LOG_DATABASE, 0, "Attempt to access field at index %u but there %s only %u field%s", index, num_fields == 1 ? "is" : "are", num_fields, num_fields == 1 ? "" : "s");
		return NULL;
	}

	return row[index];
}

const char* DatabaseResult::GetFieldValueStr(const char* field_name) {
	MYSQL_FIELD* field;
	int i = 0;

	if (!current_res) {
		return NULL;
	}

	while ((field = mysql_fetch_field(current_res)) != nullptr) {
		if (strcmp(field_name, field->name) == 0) {
			mysql_field_seek(current_res, 0);
			return row[i];
		}

		++i;
	}

	LogError(LOG_DATABASE, 0, "Attempt to access field '%s' but a field with that name was not found", field_name);
	mysql_field_seek(current_res, 0);

	return NULL;
}

bool DatabaseResult::Next() {
	return (current_res != nullptr && (row = mysql_fetch_row(current_res)) != nullptr);
}

bool DatabaseResult::NextResultSet() {
	if (results.empty()) {
		return false;
	}

	if (current_res) {
		mysql_free_result(current_res);
	}
	current_res = results.front();
	results.pop_front();

	return true;
}

bool DatabaseResult::IsNull(unsigned int index) {
	return GetFieldValue(index) == nullptr;
}

bool DatabaseResult::IsNullStr(const char* field_name) {
	return GetFieldValueStr(field_name) == nullptr;
}

uint8_t DatabaseResult::GetUInt8(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? 0 : atoi(value);
}

uint8_t DatabaseResult::GetUInt8Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? 0 : atoi(value);
}

int8_t DatabaseResult::GetInt8(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? 0 : atoi(value);
}

int8_t DatabaseResult::GetInt8Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? 0 : atoi(value);
}

uint16_t DatabaseResult::GetUInt16(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? 0 : atoi(value);
}

uint16_t DatabaseResult::GetUInt16Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? 0 : atoi(value);
}

int16_t DatabaseResult::GetInt16(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? 0 : atoi(value);
}

int16_t DatabaseResult::GetInt16Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? 0 : atoi(value);
}

uint32_t DatabaseResult::GetUInt32(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? 0U : strtoul(value, NULL, 10);
}

uint32_t DatabaseResult::GetUInt32Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? 0U : strtoul(value, NULL, 10);
}

int32_t DatabaseResult::GetInt32(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? 0 : atoi(value);
}

int32_t DatabaseResult::GetInt32Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? 0 : atoi(value);
}

uint64_t DatabaseResult::GetUInt64(unsigned int index) {
	const char* value = GetFieldValue(index);
#if defined(_WIN32)
	return value == nullptr ? 0UL : _strtoui64(value, NULL, 10);
#else
	return value == nullptr ? 0UL : strtoull(value, NULL, 10);
#endif
}

uint64_t DatabaseResult::GetUInt64Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
#if defined(_WIN32)
	return value == nullptr ? 0UL : _strtoui64(value, NULL, 10);
#else
	return value == nullptr ? 0UL : strtoull(value, NULL, 10);
#endif
}

int64_t DatabaseResult::GetInt64(unsigned int index) {
	const char* value = GetFieldValue(index);
#if defined(_WIN32)
	return value == nullptr ? 0L : _strtoi64(value, NULL, 10);
#else
	return value == nullptr ? 0L : strtoll(value, NULL, 10);
#endif
}

int64_t DatabaseResult::GetInt64Str(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
#if defined(_WIN32)
	return value == nullptr ? 0L : _strtoi64(value, NULL, 10);
#else
	return value == nullptr ? 0L : strtoll(value, NULL, 10);
#endif
}

float DatabaseResult::GetFloat(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? 0.0F : (float)atof(value);
}

float DatabaseResult::GetFloatStr(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? 0.0F : (float)atof(value);
}

char DatabaseResult::GetChar(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? '\0' : value[0];
}

char DatabaseResult::GetCharStr(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? '\0' : value[0];
}

bool DatabaseResult::GetBool(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value != nullptr && atoi(value) > 0;
}

bool DatabaseResult::GetBoolStr(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value != nullptr && atoi(value) > 0;
}

const char* DatabaseResult::GetString(unsigned int index) {
	const char* value = GetFieldValue(index);
	return value == nullptr ? empty_str : value;
}

const char* DatabaseResult::GetStringStr(const char* field_name) {
	const char* value = GetFieldValueStr(field_name);
	return value == nullptr ? empty_str : value;
}

unsigned int DatabaseResult::GetNumRows() {
	return current_res == nullptr ? 0 : (unsigned int)mysql_num_rows(current_res);
}