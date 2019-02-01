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

	typedef std::pair<int16_t, int16_t> versionRange_t;

	//This maps versions to opcodes and their packet constructors
	std::map<versionRange_t, std::map<int16_t, PacketAllocatorBase*> > versions;

	//This maps struct file names to allocators
	std::map<std::string, std::vector<PacketAllocatorBase*> > outfiles;

public:
	OpcodeManager() = default;
	~OpcodeManager() {
		for (auto& itr : allocators) {
			delete itr.second;
		}
	}

	void RegisterAllocator(const char* name, PacketAllocatorBase* allocator, std::type_index t, const char* outfile) {
		assert(allocators.count(name) == 0 && type_map.count(t) == 0);

		allocators[name] = allocator;
		type_map[t] = allocator;
		outfiles[outfile].push_back(allocator);
	}

	static OpcodeManager* GetGlobal();
	static void RegisterEmuOpcodeHelper(const char* name, PacketAllocatorBase* allocator, std::type_index t, const char* outfile);

	void RegisterVersionOpcode(const char* name, int16_t range_low, int16_t range_high, int16_t opcode) {
		auto itr = allocators.find(name);
		if (itr == allocators.end()) {
			//We don't handle this opcode currently, so don't bother making an entry
			return;
		}

		versionRange_t range = std::make_pair(range_low, range_high);
		versions[range][opcode] = itr->second;
	}

	EQ2Packet* GetPacketForVersion(int16_t version, int16_t opcode) {
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
	OpcodeRegistrar(const char* opName, const char* outfile) {
		OpcodeManager::RegisterEmuOpcodeHelper(opName, new PacketAllocator<T>, typeid(T), outfile);
	}
};

//Use this macro on a global scope to auto construct this object on program start
//n is the opcode name, pt is the packet class
#define RegisterEmuOpcode(n, pt, f) OpcodeRegistrar<pt> zUNIQUENAMEz ## pt ## (n, f)
#define RegisterWorldStruct(n, pt) RegisterEmuOpcode(n, pt, "WorldStructs.xml")