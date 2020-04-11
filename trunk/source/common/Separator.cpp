#include "stdafx.h"

#include "Separator.h"
#include "util.h"
#include "log.h"

#define SEP_MAX_INIT    10

#define IS_WHITESPACE(c) (*(c) == ' ' || *(c) == '\n' || *(c) == '\t' || *(c) == '\r')
#define IS_QUOTE(c)      (*(c) == '"')
#define IS_ESCAPE(c)     (*(c) == '\\')
#define IS_NULL(c)       (*(c) == '\0')

using namespace std;

Separator::Separator() {
	args = nullptr;
	max = SEP_MAX_INIT;
	size = 0;
}

Separator::Separator(const Separator& other) {
	size = other.size;
	max = other.max;

	if (size > 0) {
		args = static_cast<char**>(malloc(size * sizeof(char*)));

		for (int i = 0; i < size; i++) {
			args[i] = static_cast<char*>(malloc(strlen(other.args[i]) + 1));
			strcpy(args[i], other.args[i]);
		}
	}
	else {
		args = nullptr;
	}
}

Separator::Separator(Separator&& other) {
	size = other.size;
	other.size = 0;
	args = other.args;
	other.args = nullptr;
	max = other.max;
}

Separator::Separator(const std::string& str) {
	args = nullptr;
	this->max = SEP_MAX_INIT;
	size = 0;
	SetString(str.c_str());
}

Separator::~Separator() {
	if (args != nullptr) {
		for (int i = 0; i < size; i++)
			free(args[i]);
		free(args);
	}
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
	return strtoul(GetString(index), NULL, 10);
}

int Separator::GetSize() {
	return size;
}

void Separator::Clear() {
	int i;

	if (args != nullptr) {
		for (i = 0; i < size; i++)
			free(args[i]);
		free(args);

		args = nullptr;
		max = SEP_MAX_INIT;
		size = 0;
	}
}

bool Separator::Resize() {
	char **new_args;
	int new_max;

	if (max == 0)
		new_max = SEP_MAX_INIT;
	else
		new_max = max * 2;

	if ((new_args = (char **)realloc(args, new_max * sizeof(char *))) == NULL)
		return false;

	args = new_args;
	max = new_max;

	return true;
}

bool Separator::AddArg(const char *arg, int len) {
	int i, pos;

	if (args == NULL || size == max) {
		if (!Resize())
			return false;
	}

	if ((args[size] = (char *)malloc(len + 1)) == NULL)
		return false;

	pos = 0;
	for (i = 0; i < len; i++) {
		if (arg[i] == '\\' && arg[i + 1] == '"')
			continue;

		args[size][pos++] = arg[i];
	}

	//if we skipped any back slashes, pad the rest of the string with nulls
	//also make sure the extra byte allocated for the null terminator gets set
	for (i = pos; i < len + 1; i++)
		args[size][i] = '\0';

	++size;

	return true;
}

bool Separator::SetString(const char *str) {
	const char *ptr = str, *start;
	bool on_quote;

	Clear();

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

	//Free our first arg
	free(args[0]);

	//Don't worry about reallocating just move our pointers up an index
	for (int i = 1; i < size; i++) {
		args[i - 1] = args[i];
	}

	//Decrement our new arg count
	--size;
}