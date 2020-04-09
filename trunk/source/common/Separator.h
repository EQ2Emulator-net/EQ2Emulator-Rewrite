#pragma once

#include <cstdint>
#include <string>

class Separator {
public:
	Separator();
	Separator(const Separator& in);
	Separator(const std::string& str);
	~Separator();

	bool SetString(const char *str);
	void Clear();

	bool IsNumber(int index);

	const char * GetString(int index);
	int GetInt(int index);
	uint32_t GetUInt32(int index);
	float GetFloat(int index);

	int GetSize();

	void Print();

	Separator& operator=(const Separator& sep);

	void DropFirstArg();

private:
	char **args;
	int max;
	int size;

	bool Resize();
	bool AddArg(const char *arg, int len);
};
