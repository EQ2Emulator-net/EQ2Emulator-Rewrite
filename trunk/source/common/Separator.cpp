#include "stdafx.h"

#include "Separator.h"
#include "util.h"
#include "log.h"

#define IS_WHITESPACE(c) (*(c) == ' ' || *(c) == '\n' || *(c) == '\t' || *(c) == '\r')
#define IS_QUOTE(c)      (*(c) == '"')
#define IS_ESCAPE(c)     (*(c) == '\\')
#define IS_NULL(c)       (*(c) == '\0')

using namespace std;

Separator::Separator() {
	size = 0;
	bufUsed = 0;
}

Separator::Separator(const Separator& other) {
	size = other.size;
	bufUsed = other.bufUsed;

	if (size > 0) {
		memcpy(buf, other.buf,bufUsed);

		for (int i = 0; i < size; i++) {
			const char* cpyPtr = other.args[i];
			uint32_t offset = cpyPtr - other.buf;
			args[i] = buf + offset;
		}
	}

	inputString = other.inputString;
}

Separator::Separator(Separator&& other) {
	size = other.size;
	bufUsed = other.bufUsed;

	if (size > 0) {
		memcpy(buf, other.buf, bufUsed);

		for (int i = 0; i < size; i++) {
			const char* cpyPtr = other.args[i];
			uint32_t offset = cpyPtr - other.buf;
			args[i] = buf + offset;
		}
	}

	inputString = std::move(other.inputString);
}

Separator::Separator(const std::string& str) {
	bufUsed = 0;
	size = 0;
	SetString(str.c_str());
}

Separator::~Separator() {
}

const char * Separator::GetString(int index) {
	const char* ret = "";
	if (args && index < GetSize()) {
		ret = args[index];
	}
	return ret;
}

int Separator::GetInt(int index) {
	return atoi(GetString(index));
}

uint32_t Separator::GetUInt32(int index) {
	return strtoul(GetString(index), nullptr, 10);
}

int Separator::GetSize() {
	return size;
}

void Separator::Clear() {
	size = 0;
	bufUsed = 0;
}

bool Separator::AddArg(const char *arg, int len) {
	if (size == 16 || bufUsed + len + 1 > sizeof(buf))
		return false;

	char* nextPtr = buf + bufUsed;
	args[size++] = nextPtr;

	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (arg[i] == '\\' && arg[i + 1] == '"')
			continue;

		nextPtr[pos++] = arg[i];
	}

	//Make sure we null terminate the string
	nextPtr[pos++] = '\0';

	bufUsed += pos;

	return true;
}

bool Separator::SetString(const char *str) {
	const char *ptr = str, *start;
	bool on_quote;

	Clear();

	inputString = str;

	while (1) {
		on_quote = false;

		//skip front whitespace
		while (IS_WHITESPACE(ptr))
			++ptr;

		if (IS_NULL(ptr))
			break;

		if (IS_QUOTE(ptr)) {
			on_quote = true;
			++ptr;
		}

		//we are at the beginning of the arg, traverse to the end of the arg then
		//copy the arg into the struct
		start = ptr;
		if (on_quote) {
			while (1) {
				if (IS_QUOTE(ptr) && !IS_ESCAPE(ptr - 1))
					break;
				if (IS_NULL(ptr))
					break;

				++ptr;
			}
		}
		else {
			while (!IS_WHITESPACE(ptr) && !IS_NULL(ptr))
				++ptr;
		}

		if (!AddArg(start, static_cast<int>(ptr - start)))
			return false;

		//if this is a quoted arg, skip the ending quote
		if (on_quote)
			++ptr;

		//skip end whitespace
		while (IS_WHITESPACE(ptr))
			++ptr;
	}

	return true;
}

void Separator::Print() {
	int i;

	LogTrace(LOG_COMMAND, 0, "Separator (size=%d):", size);

	for (i = 0; i < size; i++)
		LogTrace(LOG_COMMAND, 0, "%02d: '%s'", i + 1, args[i]);
}

bool Separator::IsNumber(int index) {
	bool SeenDec = false;
	const char* arg = GetString(index);
	size_t len = strlen(arg);
	if (len == 0) {
		return false;
	}
	for (size_t i = 0; i < len; i++) {
		if (arg[i] < '0' || arg[i] > '9') {
			if (arg[i] == '.' && !SeenDec) {
				SeenDec = true;
			}
			else if (i == 0 && (arg[i] == '-' || arg[i] == '+') && !arg[i + 1] == 0) {
				continue;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

float Separator::GetFloat(int index) {
	return (float)atof(GetString(index));
}

void Separator::DropFirstArg() {
	if (size == 0) {
		return;
	}

	if (size == 1) {
		Clear();
		return;
	}

	//Move our pointers up an index
	for (int i = 1; i < size; i++) {
		args[i - 1] = args[i];
	}

	//Decrement our new arg count
	--size;
}

const std::string& Separator::GetInputString() {
	return inputString;
}