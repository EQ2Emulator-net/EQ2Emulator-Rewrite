#pragma once

#include <cstdint>
#include <string>

class Separator {
public:
	Separator();
	Separator(const Separator& in);
	Separator(Separator&& other);
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

	void DropFirstArg();

	std::string GetInputString();

private:
	int size;
	size_t bufUsed;
	std::string inputString;
	char buf[1024];
	char* args[16];

	bool AddArg(const char *arg, int len);
};
