#pragma once 

#include <type_traits>
#include <string>
#include <map>
#include <cassert>
#include <utility>
#include "../EQ2Packet.h"
#include <typeindex>
#include <vector>

class PacketAllocatorBase {
protected:
	PacketAllocatorBase() = default;

public:
	virtual ~PacketAllocatorBase() = default;
	virtual EQ2Packet* Create(int16_t version) = 0;
};

template <typename T>
class PacketAllocator : public PacketAllocatorBase {
	static_assert(std::is_base_of<EQ2Packet, T>::value, "Tried to create a packet constructor for a non packet type!");
public:
	PacketAllocator() = default;
	~PacketAllocator() = default;

	EQ2Packet* Create(int16_t version) {

		return new T(version);
	}
};

class OpcodeManager {
private:
	//<name, allocator>
	std::map<std::string, PacketAllocatorBase*> allocators;
	
	//<type_index, allocator>
	std::map<std::type_index, PacketAllocatorBase*> type_map;

	typedef std::pair<uint16_t, uint16_t> versionRange_t;

	//This maps versions to opcodes and their packet constructors
	std::map<versionRange_t, std::map<uint16_t, PacketAllocatorBase*> > versions;

	//This maps struct file names to allocators
	std::map<std::string, std::vector<PacketAllocatorBase*> > outfiles;

	//This maps allocators to struct versions
	std::map<PacketAllocatorBase*, std::vector<int32_t> > struct_versions;

public:
	OpcodeManager() = default;
	~OpcodeManager() {
		for (auto& itr : allocators) {
			delete itr.second;
		}
	}

	void RegisterAllocator(const char* name, PacketAllocatorBase* allocator, std::type_index t, 
		const char* outfile, const int32_t* versions, int32_t num_versions) {
		assert(allocators.count(name) == 0 && type_map.count(t) == 0);

		allocators[name] = allocator;
		type_map[t] = allocator;
		outfiles[outfile].push_back(allocator);

		std::vector<int32_t>& vec = struct_versions[allocator];
		vec.resize(num_versions);
		memcpy(vec.data(), versions, num_versions * sizeof(int32_t));
	}

	static OpcodeManager* GetGlobal();
	static void RegisterEmuOpcodeHelper(const char* name, PacketAllocatorBase* allocator, std::type_index t, 
		const char* outfile, const int32_t* versions, int32_t num_versions);

	void RegisterVersionOpcode(const char* name, uint16_t range_low, uint16_t range_high, uint16_t opcode) {
		auto itr = allocators.find(name);
		if (itr == allocators.end()) {
			//We don't handle this opcode currently, so don't bother making an entry
			return;
		}

		versionRange_t range = std::make_pair(range_low, range_high);
		versions[range][opcode] = itr->second;
	}

	EQ2Packet* GetPacketForVersion(uint16_t version, uint16_t opcode) {
		EQ2Packet* ret = nullptr;
		for (auto& itr : versions) {
			versionRange_t range = itr.first;
			if (range.first <= version && range.second >= version) {
				auto op = itr.second.find(opcode);
				if (op != itr.second.end()) {
					ret = op->second->Create(version);
					ret->opcode = opcode;
				}
				break;
			}
		}
		return ret;
	}

	bool HasVersion(uint16_t version) {
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

	bool SetOpcodeForPacket(EQ2Packet* packet) {
		auto itr = type_map.find(typeid(*packet));
		assert(("Please register this packet class with an opcode.", itr != type_map.end()));

		PacketAllocatorBase* allocator = itr->second;
		for (auto& itr : versions) {
			uint16_t version = packet->GetVersion();
			versionRange_t range = itr.first;
			if (range.first <= version && range.second >= version) {
				for (auto& op : itr.second) {
					if (op.second == allocator) {
						packet->opcode = op.first;
						return true;
					}
				}
				assert(("Could not find an opcode for this packet! Check it out.", false));
				return false;
			}
		}

		LogDebug(LOG_PACKET, 0, "Could not find a version range for version %u", packet->GetVersion());
		return false;
	}
};

template<typename T>
class OpcodeRegistrar {
	static_assert(std::is_base_of<EQ2Packet, T>::value, "Tried to register an Opcode for a non packet type!");
public:
	OpcodeRegistrar(const char* opName, const char* outfile, const int32_t* versions, int32_t version_count) {
		OpcodeManager::RegisterEmuOpcodeHelper(opName, new PacketAllocator<T>, typeid(T), outfile, versions, version_count);
	}
};

//Use this macro on a global scope to auto construct this object on program start
//n is the opcode name, pt is the packet class
#define RegisterEmuOpcode(n, pt, f, ...) int32_t zUNIQUENAMEVERz ## pt ##[] = { __VA_ARGS__ };\
OpcodeRegistrar<pt> zUNIQUENAMEz ## pt ## (n, f, zUNIQUENAMEVERz ## pt, sizeof(zUNIQUENAMEVERz ## pt) / sizeof(int32_t))
#define RegisterWorldStruct(n, pt, ...) RegisterEmuOpcode(n, pt, "WorldStructs.xml", __VA_ARGS__)