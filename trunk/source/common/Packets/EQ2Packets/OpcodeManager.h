#pragma once 

#include <type_traits>
#include <string>
#include <map>
#include <cassert>
#include <utility>
#include "../EQ2Packet.h"
#include <typeindex>
#include <vector>

class EQ2PacketAllocatorBase {
protected:
	EQ2PacketAllocatorBase(const char* pOpName) : opName(pOpName) {}

public:
	virtual ~EQ2PacketAllocatorBase() = default;
	virtual EQ2Packet* Create(uint32_t version) = 0;

	const char* opName;
};

template <typename T>
class EQ2PacketAllocator : public EQ2PacketAllocatorBase {
	static_assert(std::is_base_of<EQ2Packet, T>::value, "Tried to create a packet constructor for a non packet type!");
public:
	EQ2PacketAllocator(const char* pOpName) : EQ2PacketAllocatorBase(pOpName) {}
	~EQ2PacketAllocator() = default;

	EQ2Packet* Create(uint32_t version) {

		return new T(version);
	}
};

class OpcodeManager {
private:
	//<name, allocator>
	std::map<std::string, EQ2PacketAllocatorBase*> allocators;
	
	//<type_index, allocator>
	std::map<std::type_index, EQ2PacketAllocatorBase*> type_map;

	typedef std::pair<uint32_t, uint32_t> versionRange_t;

	//This maps versions to opcodes and their packet constructors
	std::map<versionRange_t, std::map<uint16_t, EQ2PacketAllocatorBase*> > versions;

	//This maps struct file names to packet names used in that file
	std::map<std::string, std::vector<std::string> > outfiles;

	//This maps packet names to struct versions for the packet
	std::map<std::string, std::vector<int32_t> > struct_versions;

public:
	OpcodeManager() = default;
	~OpcodeManager() {
		for (auto& itr : allocators) {
			delete itr.second;
		}
	}

	void RegisterAllocator(const char* name, EQ2PacketAllocatorBase* allocator, std::type_index t,
		const char* outfile, const uint32_t* versions, int32_t num_versions) {
		//Took the assert out for if the same type is registered twice...
		//but you must use a new type if writing the packet for FindOpcode
		assert(allocators.count(name) == 0/* && type_map.count(t) == 0*/);

		allocators[name] = allocator;
		type_map[t] = allocator;
		outfiles[outfile].push_back(name);

		const char* struct_prefix = "WS_";
		if (strcmp(outfile, "LoginStructs.xml") == 0) {
			struct_prefix = "LS_";
		}

		std::string struct_name = name;
		struct_name.replace(0, 3, struct_prefix);
		std::vector<int32_t>& vec = struct_versions[struct_name];
		vec.resize(num_versions);
		memcpy(vec.data(), versions, num_versions * sizeof(int32_t));
	}

	static OpcodeManager* GetGlobal();
	static void RegisterEQ2OpcodeHelper(const char* name, EQ2PacketAllocatorBase* allocator, std::type_index t,
		const char* outfile, const uint32_t* versions, int32_t num_versions);

	void RegisterVersionOpcode(const char* name, uint32_t range_low, uint32_t range_high, uint16_t opcode) {
		auto itr = allocators.find(name);
		if (itr == allocators.end()) {
			//We don't handle this opcode currently, so don't bother making an entry
			return;
		}

		versionRange_t range = std::make_pair(range_low, range_high);
		versions[range][opcode] = itr->second;
	}

	EQ2Packet* GetPacketForVersion(uint32_t version, uint16_t opcode);

	bool HasVersion(uint32_t version) {
		bool ret = false;
		for (auto& itr : versions) {
			versionRange_t range = itr.first;
			if (range.first <= version && range.second >= version) {
				ret = true;
				break;
			}
		}
		return ret;
	}

	bool SetOpcodeForPacket(EQ2Packet* packet);
};

template<typename T>
class OpcodeRegistrar {
	static_assert(std::is_base_of<EQ2Packet, T>::value, "Tried to register an Opcode for a non packet type!");
public:
	OpcodeRegistrar(const char* opName, const char* outfile, const uint32_t* versions, int32_t version_count) {
		OpcodeManager::RegisterEQ2OpcodeHelper(opName, new EQ2PacketAllocator<T>(opName), typeid(T), outfile, versions, version_count);
	}
};

//Use this macro on a global scope to auto construct this object on program start
//n is the opcode name, pt is the packet class
#define RegisterEQ2Opcode(n, pt, f, ...) uint32_t zUNIQUENAMEVERz ## pt [] = { __VA_ARGS__ };\
OpcodeRegistrar<pt> zUNIQUENAMEz ## pt (n, f, zUNIQUENAMEVERz ## pt, sizeof(zUNIQUENAMEVERz ## pt) / sizeof(uint32_t))
#define RegisterWorldStruct(n, pt, ...) RegisterEQ2Opcode(n, pt, "WorldStructs.xml", __VA_ARGS__)
#define RegisterLoginStruct(n, pt, ...) RegisterEQ2Opcode(n, pt, "LoginStructs.xml", __VA_ARGS__)
#define RegisterZoneStruct(n, pt, ...) RegisterEQ2Opcode(n, pt, "ZoneStruct.xml", __VA_ARGS__)