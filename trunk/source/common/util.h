#pragma once

#include <stdint.h>
#include <time.h>
#include <string>
#if defined(_WIN32)
#include <WinSock2.h>
#else
# include <unistd.h>
#include <arpa/inet.h>
#endif

#if defined(_WIN32)
typedef int pid_t;
#endif

void SleepMS(unsigned int milliseconds);
pid_t GetPID();

bool IsUInt(const char *str);

//TODO: move this to some game time module or something
const char * GetElapsedTime(time_t seconds, char *dst, unsigned int size);

void DumpBytes(const unsigned char* bytes, unsigned int len, const char* header = nullptr, bool bTrace = false);
void DumpBytes(const char *bytes, unsigned int len);

void UpdateWindowTitle(const char *title);

int Deflate(unsigned char* in_data, int in_length, unsigned char* out_data, int max_out_length);
int Inflate(unsigned char* indata, int indatalen, unsigned char* outdata, int outdatalen, bool iQuiet = true);

std::string appStrError(int err_num = 0);
bool IsUnsignedInt(const char *str);

bool alpha_check(unsigned char val);

#define atoul(str) strtoul(str, NULL, 10)

float MakeRandom(float low, float high);
int32_t MakeRandomInt(int32_t low, int32_t high);
uint32_t MakeRandomNumber();

//This will compare a weak_ptr to either a shared_ptr or weak_ptr
#define EmuWeakCmp(lhs, rhs) (!(lhs.owner_before(rhs) || rhs.owner_before(lhs)))

class CStringCmpNoCopy {
public:
	CStringCmpNoCopy(const char* v) : myVal(v) {}
	~CStringCmpNoCopy() = default;
	const char* myVal;

	bool operator==(const char* rhs) {
		return strcmp(myVal, rhs) == 0;
	}
};

bool GetFileAsString(std::string& out, const char* fileName);

//This is a copy of boost::hash_combine
template<typename T>
void CombineHash(uint32_t& seed, const T& v) {
	static const std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

bool StringEndsWith(const std::string& s, const std::string& ending);