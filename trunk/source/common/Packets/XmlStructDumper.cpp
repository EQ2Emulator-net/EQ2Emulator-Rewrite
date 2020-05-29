#include "stdafx.h"

#include "XmlStructDumper.h"
#include "PacketElements/PacketPackedData.h"
#include "PacketElements/PacketEncodedData.h"
#include "EQ2Packets/OpcodeManager.h"
#include "../../depends/rapid-xml/rapidxml_print.hpp"
#include <fstream>
#ifdef EQ2_ZONE
#include "../../ZoneServer/Packets/OP_ClientCmdMsg_Packet.h"
#include "../../ZoneServer/Packets/OP_UpdateCharacterSheetMsg_Packet.h"
#endif

using namespace rapidxml;

const char* XmlStructDumper::GetElementType(PacketElement* e) {
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
	else if (auto ob = dynamic_cast<PacketOversizedByte*>(e)) {
		return ob->bSigned ? "sint16" : "int16";
	}
	else if (dynamic_cast<PacketBool*>(e)) {
		return "bool";
	}
	else if (dynamic_cast<PacketSubstruct*>(e)) {
		return "substruct";
	}
	else if (dynamic_cast<PacketEQ2EquipmentItem*>(e)) {
		return "EQ2_EquipmentItem";
	}

	//unknown type?
	assert(false);
	return "";
}

std::deque<xml_node<>*> XmlStructDumper::SubstructToXml(xml_document<>& doc, const std::string& name) {
	SubstructAllocatorBase* sa = nullptr;
	std::vector<uint32_t> versions;

	{
		SubstructManager* subman = SubstructManager::GetGlobal();
		auto itr = subman->allocators.find(name);
		assert(itr != subman->allocators.end());
		sa = itr->second.get();
		versions = subman->struct_versions[name];
		assert(!versions.empty());
	}

	std::deque<xml_node<>*> ret;

	for (auto& itr : versions) {
		xml_node<>* structNode = doc.allocate_node(node_element, "Struct");
		structNode->append_attribute(doc.allocate_attribute("Name", doc.allocate_string(name.c_str())));
		structNode->append_attribute(doc.allocate_attribute("ClientVersion", doc.allocate_string(std::to_string(itr).c_str())));
		
		std::unique_ptr<PacketSubstruct> ps(sa->Create(itr));

		ps->CheckElementsInitialized();

		assert(!ps->bInline);
		
		for (auto& e : ps->elements) {
			ElementToXml(e, doc, *structNode);
		}

		ret.push_back(structNode);
	}

	return ret;
}

void XmlStructDumper::ElementToXml(PacketElement* e, xml_document<>& doc, xml_node<>& parent) {
	if (e->bHidden) {
		return;
	}

	xml_node<>* dataNode = doc.allocate_node(node_element, "Data");
	if (auto pe = dynamic_cast<PacketPackedData*>(e)) {
		dataNode->append_attribute(doc.allocate_attribute("Type", "PackedData"));
		dataNode->append_attribute(doc.allocate_attribute("IncludeSize", pe->bIncludeSize ? "true" : "false"));
		if (e->ifVariableSet) {
			dataNode->append_attribute(doc.allocate_attribute("IfVariableSet", doc.allocate_string(e->ifVariableSet->name)));
		}
		for (auto& itr : static_cast<PacketSubstruct*>(e)->elements) {
			ElementToXml(itr, doc, *dataNode);
		}
	}
	else if (auto pa = dynamic_cast<PacketArrayBase*>(e)) {
		dataNode->append_attribute(doc.allocate_attribute("ElementName", doc.allocate_string(e->name)));
		dataNode->append_attribute(doc.allocate_attribute("Type", "Array"));
		dataNode->append_attribute(doc.allocate_attribute("ArraySizeVariable", pa->arraySizeName));
		std::unique_ptr<PacketSubstruct> arrayType = pa->GetArraySubstruct();
		//This default array data name is only used if this is not an inline packet struct
		std::string defaultArrayDataName = std::string(e->name) + "_data";
		arrayType->SetName(defaultArrayDataName.c_str());
		ElementToXml(arrayType.get(), doc, *dataNode);
	}
	else if (auto ps = dynamic_cast<PacketSubstruct*>(e)) {
		//This is is encoded data add an extra tag

		//Do I really want this?

		//bool bEncoded = false;

		//if (auto pe = dynamic_cast<PacketEncodedData*>(e)) {
		//	dataNode->append_attribute(doc.allocate_attribute("Type", "EncodedData"));
		//	for (auto& itr : static_cast<PacketSubstruct*>(e)->elements) {
		//		ElementToXml(itr, doc, *dataNode);
		//	}
		//}

		if (ps->bInline) {
			ps->CheckElementsInitialized();
			for (auto& e : ps->elements) {
				ElementToXml(e, doc, parent);
			}
			//If we're inline we don't want to add a new node here, just use the parent and return before a new node is appended
			return;
		}
		else {
			std::string psName = SubstructManager::GetGlobal()->GetSubstructName(typeid(*ps));
			dataNode->append_attribute(doc.allocate_attribute("ElementName", doc.allocate_string(e->name)));
			dataNode->append_attribute(doc.allocate_attribute("Substruct", psName.empty() ? "REGISTER_MY_SUBSTRUCT" : doc.allocate_string(psName.c_str())));
			dataNode->append_attribute(doc.allocate_attribute("Size", doc.allocate_string(std::to_string(e->count).c_str())));
			if (e->ifVariableSet) {
				dataNode->append_attribute(doc.allocate_attribute("IfVariableSet", doc.allocate_string(e->ifVariableSet->name)));
			}
		}
	}
	else if (auto pp = dynamic_cast<PacketSubstructParentBase*>(e)) {
		PacketSubstruct* ps = pp->GetSubstructs();
		ps->name = e->name;
		ps->ifVariableSet = e->ifVariableSet;
		ps->count = e->count;
		ElementToXml(ps, doc, parent);
		return;
	}
	else {
		dataNode->append_attribute(doc.allocate_attribute("ElementName", doc.allocate_string(e->name)));
		dataNode->append_attribute(doc.allocate_attribute("Type", GetElementType(e)));
		dataNode->append_attribute(doc.allocate_attribute("Size", doc.allocate_string(std::to_string(e->count).c_str())));
		if (auto oe = dynamic_cast<PacketOversizedByte*>(e)) {
			dataNode->append_attribute(doc.allocate_attribute("OversizedValue", oe->bSigned ? "127" : "255"));
		}
		if (auto ei = dynamic_cast<PacketEQ2EquipmentItem*>(e)) {
			dataNode->append_attribute(doc.allocate_attribute("bShortType", ei->bShortType ? "true" : "false"));
		}
	}
	if (e->check_element) {
		std::ostringstream ss;
		ss << e->check_element->name << (e->equality_type ? "=" : "!=") << e->check_value;
		dataNode->append_attribute(doc.allocate_attribute("Criteria", doc.allocate_string(ss.str().c_str())));
	}
	if (e->ifVariableSet) {
		dataNode->append_attribute(doc.allocate_attribute("IfVariableSet", doc.allocate_string(e->ifVariableSet->name)));
	}
	if (!e->ifAnyVariableSet.empty()) {
		bool bFirst = true;
		std::ostringstream ss;
		for (auto& itr : e->ifAnyVariableSet) {
			if (bFirst) {
				bFirst = false;
			}
			else {
				ss << ',';
			}
			ss << itr->name;
		}

		dataNode->append_attribute(doc.allocate_attribute("IfAnyVariableSet", doc.allocate_string(ss.str().c_str())));
	}
	parent.append_node(dataNode);
}

std::string SubstructManager::GetSubstructName(const std::type_info& t) {
	std::string ret;
	auto itr = struct_name_map.find(&t);
	if (itr != struct_name_map.end()) {
		ret = itr->second;
	}
#ifdef EQ2_ZONE
	else if (t == typeid(OP_UpdateCharacterSheetMsg_Packet)) {
		ret = "UpdateCharacterSheetMsgData";
	}
#endif
	return ret;
}

void XmlStructDumper::DumpSubstructsToFile(const char* filename, xml_document<>& doc, xml_node<>& root) {
	SubstructManager* subman = SubstructManager::GetGlobal();
	std::vector<std::string> structNames = subman->outfiles[filename];

	for (auto& itr : structNames) {
		auto versionedNodes = SubstructToXml(doc, itr);
		for (auto& node : versionedNodes) {
			root.append_node(node);
		}
	}
}

void XmlStructDumper::DumpStructsFile(const char* filename) {
	std::unique_ptr<xml_document<>> doc(new xml_document<>);
	xml_node<>* root = doc->allocate_node(node_element, "EQ2Emulator");
	DumpSubstructsToFile(filename, *doc, *root);
	DumpPacketStructsToFile(filename, *doc, *root);
	doc->append_node(root);

	std::ofstream outfile(filename, std::ios::trunc);
	outfile << *doc;
}

std::deque<xml_node<>*> XmlStructDumper::PacketStructToXml(xml_document<>& doc, const std::string& name) {
	EQ2PacketAllocatorBase* pa = nullptr;
	std::vector<uint32_t> versions;

	{
		OpcodeManager* opman = OpcodeManager::GetGlobal();
		std::string opName = name;
		opName.replace(0, 3, "OP_");
		auto itr = opman->allocators.find(opName);
		assert(itr != opman->allocators.end());
		pa = itr->second;
		versions = opman->struct_versions[name];
		assert(!versions.empty());
	}

	std::deque<xml_node<>*> ret;

	for (auto& itr : versions) {
		xml_node<>* structNode = doc.allocate_node(node_element, "Struct");
		structNode->append_attribute(doc.allocate_attribute("Name", doc.allocate_string(name.c_str())));
		structNode->append_attribute(doc.allocate_attribute("ClientVersion", doc.allocate_string(std::to_string(itr).c_str())));

		std::unique_ptr<EQ2Packet> ps(pa->Create(itr));

#ifdef EQ2_ZONE
		if (dynamic_cast<OP_ClientCmdMsg_Packet*>(ps.get())) {
			if (strcmp(pa->opName, "OP_ClientCmdMsg") != 0) {
				structNode->append_attribute(doc.allocate_attribute("OpcodeName", "OP_ClientCmdMsg"));
				structNode->append_attribute(doc.allocate_attribute("OpcodeType", pa->opName));
			}
			else {
				return ret;
			}
		}
		else 
#endif
			//This next statement is attached to the above else for zoneserver but not other builds
			structNode->append_attribute(doc.allocate_attribute("OpcodeName", pa->opName));

		for (auto& e : ps->elements) {
			ElementToXml(e, doc, *structNode);
		}

		ret.push_back(structNode);
	}

	return ret;
}

void XmlStructDumper::DumpPacketStructsToFile(const char* filename, xml_document<>& doc, xml_node<>& root) {
	OpcodeManager* opman = OpcodeManager::GetGlobal();
	std::vector<std::string> structNames = opman->outfiles[filename];

	for (auto& itr : structNames) {
		auto structNodes = PacketStructToXml(doc, itr);
		for (auto& node : structNodes) {
			root.append_node(node);
		}
	}
}