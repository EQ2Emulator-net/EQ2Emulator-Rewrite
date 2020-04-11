#pragma once

#include "PacketElements/PacketElements.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

class SubstructAllocatorBase {
protected:
	SubstructAllocatorBase(const char* pName) : structName(pName) {}

public:
	virtual ~SubstructAllocatorBase() = default;
	virtual PacketSubstruct* Create(uint32_t version) = 0;

	const char* structName;
};

template <typename T>
class SubstructAllocator : public SubstructAllocatorBase {
	static_assert(std::is_base_of<PacketSubstruct, T>::value, "Tried to create a subtruct constructor for a non substruct type!");
public:
	SubstructAllocator(const char* pOpName) : SubstructAllocatorBase(pOpName) { }
	~SubstructAllocator() = default;

	PacketSubstruct* Create(uint32_t version) {
		return new T(version);
	}
};

class SubstructManager {
public:
	~SubstructManager() = default;

	static SubstructManager* GetGlobal() {
		static SubstructManager g_manager;
		return &g_manager;
	}

	//This maps packet names to struct versions for the struct
	std::map<std::string, std::vector<int32_t> > struct_versions;

	//<name, allocator>
	std::map<std::string, std::unique_ptr<SubstructAllocatorBase>> allocators;

	//This maps struct file names to packet names used in that file
	std::map<std::string, std::vector<std::string> > outfiles;

	void RegisterAllocator(const char* name, SubstructAllocatorBase* allocator, const char* outfile, const uint32_t* versions, int32_t num_versions) {
		//Took the assert out for if the same type is registered twice...
		//but you must use a new type if writing the packet for FindOpcode
		assert(allocators.count(name) == 0/* && type_map.count(t) == 0*/);

		allocators.emplace(name, allocator);
		outfiles[outfile].push_back(name);

		std::vector<int32_t>& vec = struct_versions[name];
		vec.resize(num_versions);
		memcpy(vec.data(), versions, num_versions * sizeof(int32_t));
	}

private:
	SubstructManager() = default;

};

template<typename T>
class SubstructRegistrar {
	static_assert(std::is_base_of<SubstructRegistrar, T>::value, "Tried to register a non substruct type!");
public:
	SubstructRegistrar(const char* Name, const char* outfile, const uint32_t* versions, int32_t version_count) {
		OpcodeManager::RegisterEQ2OpcodeHelper(opName, new SubstructAllocator<T>(opName), typeid(T), outfile, versions, version_count);
	}
};

class XmlStructDumper {
public:

	static const char* GetElementType(PacketElement* e) {
		if (dynamic_cast<PacketUInt8*>(e)) {
			return "int8";
		}
		else if (dynamic_cast<PacketInt8*>(e)) {
			return "sint8";
		}
		else if (dynamic_cast<PacketUInt16*>(e)) {
			return "int16";
		}
		else if (dynamic_cast<PacketInt16*>(e)) {
			return "sint16";
		}
		else if (dynamic_cast<PacketUInt32*>(e)) {
			return "int32";
		}
		else if (dynamic_cast<PacketInt32*>(e)) {
			return "sint32";
		}
		else if (dynamic_cast<PacketUInt64*>(e)) {
			return "int64";
		}
		else if (dynamic_cast<PacketInt64*>(e)) {
			return "sint64";
		}
		else if (dynamic_cast<PacketFloat*>(e)) {
			return "float";
		}
		else if (dynamic_cast<PacketDouble*>(e)) {
			return "double";
		}
		else if (dynamic_cast<PacketEQ2Color*>(e)) {
			return "EQ2_Color";
		}
		else if (dynamic_cast<PacketEQ2ColorFloat*>(e)) {
			return "EQ2_Color_Float";
		}
		else if (dynamic_cast<Packet8String*>(e)) {
			return "EQ2_8Bit_String";
		}
		else if (dynamic_cast<Packet16String*>(e)) {
			return "EQ2_16Bit_String";
		}
		else if (dynamic_cast<Packet32String*>(e)) {
			return "EQ2_32Bit_String";
		}
		else if (dynamic_cast<PacketChar*>(e)) {
			return "char";
		}
		else if (dynamic_cast<PacketBool*>(e)) {
			return "bool";
		}
		else if (dynamic_cast<PacketOversizedByte*>(e)) {
			return "int16";
		}
		else if (dynamic_cast<PacketBool*>(e)) {
			return "bool";
		}
		else if (dynamic_cast<PacketSubstruct*>(e)) {
			return "substruct";
		}
	}
};