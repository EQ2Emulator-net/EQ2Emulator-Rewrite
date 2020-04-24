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
	char **args;
	int max;
	int size;
	std::string inputString;

	bool Resize();
	bool AddArg(const char *arg, int len);
};
