#include "stdafx.h"

#include "database-result.h"

//return this instead of NULL for certain functions to prevent crashes from coding errors
static const char *empty_str = "";

DatabaseResult::DatabaseResult() {
	result = NULL;
}

DatabaseResult::~DatabaseResult() {
	if (result != NULL)
		mysql_free_result(result);
}

void DatabaseResult::SetResult(MYSQL_RES *res) {
	if (result != NULL)
		mysql_free_result(result);

	result = res;
}

const char * DatabaseResult::GetFieldValue(unsigned int index) {
	if (index >= mysql_num_fields(result))
		return NULL;

	return row[index];
}

const char * DatabaseResult::GetFieldValueStr(const char *field_name) {
	MYSQL_FIELD *field;
	int i = 0;

	while ((field = mysql_fetch_field(result)) != NULL) {
		if (strncmp(field_name, field->name, field->name_length) == 0) {
			mysql_field_seek(result, 0);
			return row[i];
		}

		++i;
	}

	mysql_field_seek(result, 0);

	return NULL;
}

bool DatabaseResult::Next() {
	return (result != NULL && (row = mysql_fetch_row(result)) != NULL);
}

bool DatabaseResult::IsNull(unsigned int index) {
	return GetFieldValue(index) == NULL;
}

bool DatabaseResult::IsNullStr(const char *field_name) {
	return GetFieldValueStr(field_name) == NULL;
}

uint8_t DatabaseResult::GetUInt8(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0 : atoi(value);
}

uint8_t DatabaseResult::GetUInt8Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0 : atoi(value);
}

int8_t DatabaseResult::GetInt8(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0 : atoi(value);
}

int8_t DatabaseResult::GetInt8Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0 : atoi(value);
}

uint16_t DatabaseResult::GetUInt16(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0 : atoi(value);
}

uint16_t DatabaseResult::GetUInt16Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0 : atoi(value);
}

int16_t DatabaseResult::GetInt16(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0 : atoi(value);
}

int16_t DatabaseResult::GetInt16Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0 : atoi(value);
}

uint32_t DatabaseResult::GetUInt32(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0U : strtoul(value, NULL, 10);
}

uint32_t DatabaseResult::GetUInt32Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0U : strtoul(value, NULL, 10);
}

unsigned int DatabaseResult::GetUInt(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0U : strtoul(value, NULL, 10);
}

unsigned int DatabaseResult::GetUIntStr(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0U : strtoul(value, NULL, 10);
}

int32_t DatabaseResult::GetInt32(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0 : atoi(value);
}

int32_t DatabaseResult::GetInt32Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0 : atoi(value);
}

uint64_t DatabaseResult::GetUInt64(unsigned int index) {
	const char *value = GetFieldValue(index);
#if defined(_WIN32)
	return value == NULL ? 0UL : _strtoui64(value, NULL, 10);
#else
	return value == NULL ? 0UL : strtoull(value, NULL, 10);
#endif
}

uint64_t DatabaseResult::GetUInt64Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
#if defined(_WIN32)
	return value == NULL ? 0UL : _strtoui64(value, NULL, 10);
#else
	return value == NULL ? 0UL : strtoull(value, NULL, 10);
#endif
}

int64_t DatabaseResult::GetInt64(unsigned int index) {
	const char *value = GetFieldValue(index);
#if defined(_WIN32)
	return value == NULL ? 0L : _strtoi64(value, NULL, 10);
#else
	return value == NULL ? 0L : strtoll(value, NULL, 10);
#endif
}

int64_t DatabaseResult::GetInt64Str(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
#if defined(_WIN32)
	return value == NULL ? 0L : _strtoi64(value, NULL, 10);
#else
	return value == NULL ? 0L : strtoll(value, NULL, 10);
#endif
}

float DatabaseResult::GetFloat(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? 0.0F : (float)atof(value);
}

float DatabaseResult::GetFloatStr(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? 0.0F : (float)atof(value);
}

char DatabaseResult::GetChar(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? '\0' : value[0];
}

char DatabaseResult::GetCharStr(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? '\0' : value[0];
}

bool DatabaseResult::GetBool(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value != NULL && atoi(value) > 0;
}

bool DatabaseResult::GetBoolStr(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value != NULL && atoi(value) > 0;
}

const char * DatabaseResult::GetString(unsigned int index) {
	const char *value = GetFieldValue(index);
	return value == NULL ? empty_str : value;
}

const char * DatabaseResult::GetStringStr(const char *field_name) {
	const char *value = GetFieldValueStr(field_name);
	return value == NULL ? empty_str : value;
}

unsigned int DatabaseResult::GetNumRows() {
	return result == NULL ? 0 : (unsigned int)mysql_num_rows(result);
}
