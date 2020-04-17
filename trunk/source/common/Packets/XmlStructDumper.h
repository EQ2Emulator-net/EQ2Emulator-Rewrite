#pragma once

#include "PacketElements/PacketElements.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <deque>
#include "../../depends/rapid-xml/rapidxml.hpp"
#include <set>
#include <typeinfo>

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
	std::map<std::string, std::vector<uint32_t> > struct_versions;

	//<name, allocator>
	std::map<std::string, std::unique_ptr<SubstructAllocatorBase>> allocators;

	//This maps struct file names to packet names used in that file
	std::map<std::string, std::vector<std::string> > outfiles;

	//<typeid, name>
	std::map<const std::type_info*, std::string> struct_name_map;

	std::string GetSubstructName(const std::type_info& t);

	void RegisterAllocator(const char* name, SubstructAllocatorBase* allocator, const std::type_info& t, const char* outfile, const uint32_t* versions, int32_t num_versions) {
		//Took the assert out for if the same type is registered twice...
		//but you must use a new type if writing the packet for FindOpcode
		assert(allocators.count(name) == 0);

		allocators.emplace(name, allocator);
		outfiles[outfile].push_back(name);

		struct_name_map[&t] = name;

		std::vector<uint32_t>& vec = struct_versions[name];
		vec.resize(num_versions);
		memcpy(vec.data(), versions, num_versions * sizeof(int32_t));
	}

private:
	SubstructManager() = default;

};

template<typename T>
class SubstructRegistrar {
	static_assert(std::is_base_of<PacketSubstruct, T>::value, "Tried to register a non substruct type!");
public:
	SubstructRegistrar(const char* Name, const char* outfile, const uint32_t* versions, int32_t version_count) {
		SubstructManager::GetGlobal()->RegisterAllocator(Name, new SubstructAllocator<T>(Name), typeid(T), outfile, versions, version_count);
	}
};

class XmlStructDumper {
public:
	XmlStructDumper() = default;
	~XmlStructDumper() = default;

	void DumpStructsFile(const char* filename);

private:
	std::deque<rapidxml::xml_node<>*> PacketStructToXml(rapidxml::xml_document<>& doc, const std::string& name);
	std::deque<rapidxml::xml_node<>*> SubstructToXml(rapidxml::xml_document<>& doc, const std::string& name);
	static void ElementToXml(PacketElement* e, rapidxml::xml_document<>& doc, rapidxml::xml_node<>& parent);
	static const char* GetElementType(PacketElement* e);

	void DumpSubstructsToFile(const char* filename, rapidxml::xml_document<>& doc, rapidxml::xml_node<>& root);
	void DumpPacketStructsToFile(const char* filename, rapidxml::xml_document<>& doc, rapidxml::xml_node<>& root);
};

#define RegisterXmlSubstruct(pt, f, ...) uint32_t zUNIQUENAMEVERz ## pt [] = { __VA_ARGS__ };\
SubstructRegistrar<pt> zUNIQUENAMEz ## pt (#pt, f, zUNIQUENAMEVERz ## pt, sizeof(zUNIQUENAMEVERz ## pt) / sizeof(uint32_t))

