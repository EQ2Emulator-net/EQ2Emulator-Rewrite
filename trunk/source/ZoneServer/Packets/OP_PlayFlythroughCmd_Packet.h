#pragma once

#include "OP_ClientCmdMsg_Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../Spawns/SpawnStructs.h"
#include <variant>

class Substruct_FlythroughDesc_Action_LensFilter : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_LensFilter(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_LensFilter() = default;

	void RegisterElements() override {
		RegisterFloat(x1);
		RegisterFloat(y1);
		RegisterFloat(z1);
		RegisterFloat(x2);
		RegisterFloat(y2);
		RegisterFloat(z2);
	}

	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
};

class Substruct_FlythroughDesc_Action_LensFilterState : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_LensFilterState(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_LensFilterState() = default;

	void RegisterElements() override {
		RegisterUInt8(state);
	}

	uint8_t state;
};

class Substruct_FlythroughDesc_Action_SceneFade : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_SceneFade(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_SceneFade() = default;

	void RegisterElements() override {
		RegisterInt32(unknown1);
		RegisterInt32(unknown2);
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
	}

	int32_t unknown1;
	int32_t unknown2;
	float x;
	float y;
	float z;
};

class Substruct_FlythroughDesc_Action_SetCamera : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_SetCamera(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_SetCamera() = default;

	void RegisterElements() override {
		RegisterInt32(camera);
	}

	int32_t camera;
};

class Substruct_FlythroughDesc_Action_DeactivateCamera : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_DeactivateCamera(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_DeactivateCamera() = default;

	void RegisterElements() override {
		RegisterInt32(camera);
	}

	int32_t camera;
};

class Substruct_FlythroughDesc_Action_ActivateCamera : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_ActivateCamera(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_ActivateCamera() = default;

	void RegisterElements() override {
		RegisterInt32(camera);
	}

	int32_t camera;
};

class Substruct_FlythroughDesc_Action_Text : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_Text(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_Text() = default;

	void RegisterElements() override {
		Register16String(text);
	}

	std::string text;
};

class Substruct_FlythroughDesc_Action_PlaySound : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action_PlaySound(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Action_PlaySound() = default;

	void RegisterElements() override {
		Register16String(sound);
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
	}

	std::string sound;
	float x;
	float y;
	float z;
};

class Substruct_FlythroughDesc_Action : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Action(uint32_t ver = 0) : PacketSubstruct(ver), actionStruct(nullptr) {

	}

	~Substruct_FlythroughDesc_Action() = default;

	void RegisterElements() override {
		RegisterUInt32(actionType);
	}

	/*Action Types
	0 = PlaySound
	1 = Text
	2 = ActivateCamera
	3 = DeactivateCamera
	4 = SetCurrentCamera
	5 = SceneFade
	6 = LensFilterState
	7 = LensFilter*/
	uint32_t actionType;

private:
	//Magic, basically a union for classes to save on memory
	//Access from the actionStruct pointer
	std::variant<std::monostate, 
		Substruct_FlythroughDesc_Action_PlaySound, Substruct_FlythroughDesc_Action_Text,
		Substruct_FlythroughDesc_Action_ActivateCamera, Substruct_FlythroughDesc_Action_DeactivateCamera,
		Substruct_FlythroughDesc_Action_SetCamera, Substruct_FlythroughDesc_Action_SceneFade,
		Substruct_FlythroughDesc_Action_LensFilterState, Substruct_FlythroughDesc_Action_LensFilter> actionData;
public:

	PacketSubstruct* actionStruct;

	void InitActionStruct() {
		auto Helper = [this]() -> PacketSubstruct* {
			uint32_t ver = this->GetVersion();
			switch (actionType) {
			case 0:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_PlaySound>(ver);
			case 1:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_Text>(ver);
			case 2:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_ActivateCamera>(ver);
			case 3:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_DeactivateCamera>(ver);
			case 4:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_SetCamera>(ver);
			case 5:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_SceneFade>(ver);
			case 6:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_LensFilterState>(ver);
			case 7:
				return &actionData.emplace<Substruct_FlythroughDesc_Action_LensFilter>(ver);
			default:
				return nullptr;
			}
		};

		actionStruct = Helper();
	}

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		bool bSuccess = PacketSubstruct::ReadElement(srcbuf, offset, bufsize);

		if (bSuccess) {
			InitActionStruct();
			if (actionStruct) return actionStruct->ReadElement(srcbuf, offset, bufsize);
		}
		else {
			actionStruct = nullptr;
		}

		return bSuccess;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		PacketSubstruct::WriteElement(outbuf, offset);
		if (actionStruct) {
			actionStruct->WriteElement(outbuf, offset);
		}
	}

	uint32_t GetSize() override {
		uint32_t size = PacketSubstruct::GetSize();
		if (actionStruct) {
			size += actionStruct->GetSize();
		}
		return size;
	}
};

class Substruct_FlythroughDesc_Shot : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Shot(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Shot() = default;

	void RegisterElements() override {
		RegisterInt32(unknown1);
		auto e = RegisterUInt8(actionCount);
		e->SetMyArray(RegisterArray(actions, Substruct_FlythroughDesc_Action));
	}

	int32_t unknown1;
	uint8_t actionCount;
	std::vector<Substruct_FlythroughDesc_Action> actions;
};

class Substruct_FlythroughDesc_Movement_Strings : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Movement_Strings(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc_Movement_Strings() = default;

	int32_t unknown1;
	int32_t unknown2;
	std::string string1;
	std::string string2;

	void RegisterElements() override {
		RegisterInt32(unknown1);
		Register16String(string1);
		auto e = RegisterInt32(unknown2);

		std::string& string2_0 = string2;
		std::string& string2_1 = string2;
		//Hack for if <= 1
		Register16String(string2_0)->SetIfVariableEquals(0, e);
		Register16String(string2_1)->SetIfVariableEquals(1, e);
	}
};

class Substruct_FlythroughDesc_Movement : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Movement(uint32_t ver = 0) : PacketSubstruct(ver), moveStruct(nullptr) {

	}

	~Substruct_FlythroughDesc_Movement() = default;

	uint32_t moveType;

	void RegisterElements() override {
		RegisterUInt32(moveType);
	}

private:
	std::variant<std::monostate, Substruct_Lerp, Substruct_Position, Substruct_FlythroughDesc_Movement_Strings> moveData;

public:
	PacketSubstruct* moveStruct;

	void InitMoveStruct() {
		auto Helper = [this]() -> PacketSubstruct* {
			uint32_t ver = this->GetVersion();
			switch (moveType) {
			case 0:
				return &moveData.emplace<Substruct_FlythroughDesc_Movement_Strings>(ver);
			case 1:
				return &moveData.emplace<Substruct_Lerp>(ver);
			case 2:
			case 3:
				return &moveData.emplace<Substruct_Position>(ver);
			default:
				return nullptr;
			}
		};

		moveStruct = Helper();
	}

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		bool bSuccess = PacketSubstruct::ReadElement(srcbuf, offset, bufsize);

		if (bSuccess) {
			InitMoveStruct();
			if (moveStruct) return moveStruct->ReadElement(srcbuf, offset, bufsize);
		}

		return bSuccess;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		PacketSubstruct::WriteElement(outbuf, offset);
		if (moveStruct) {
			moveStruct->WriteElement(outbuf, offset);
		}
	}

	uint32_t GetSize() override {
		uint32_t size = PacketSubstruct::GetSize();
		if (moveStruct) {
			size += moveStruct->GetSize();
		}
		return size;
	}
};

class Substruct_FlythroughDesc_Scene : public PacketSubstruct {
public:
	Substruct_FlythroughDesc_Scene(uint32_t ver = 0) : PacketSubstruct(ver), move1(ver), move2(ver) {

	}

	~Substruct_FlythroughDesc_Scene() = default;

	void RegisterElements() override {
		auto e = RegisterUInt16(shotCount);
		e->SetMyArray(RegisterArray(shots, Substruct_FlythroughDesc_Shot));
		RegisterSubstruct(move1);
		RegisterSubstruct(move2);
		RegisterInt32(unknown1);
		RegisterUInt8(unknown2);
		RegisterUInt8(unknown3);
		RegisterUInt8(unknown4);
	}

	std::vector<Substruct_FlythroughDesc_Shot> shots;
	int32_t unknown1;
	uint16_t shotCount;
	uint8_t unknown2;
	uint8_t unknown3;
	uint8_t unknown4;

	Substruct_FlythroughDesc_Movement move1;
	Substruct_FlythroughDesc_Movement move2;
};

class Substruct_FlythroughDesc : public PacketSubstruct {
public:
	Substruct_FlythroughDesc(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_FlythroughDesc() = default;

	std::string unkString1;
	std::string unkString2;
	uint64_t unknown1;
	float unknown2[5];
	int32_t unknown4;
	uint8_t unknown5[5];
	uint8_t unknown3;
	bool unknown6;
	
	vec3 unknownVec;
	std::vector<Substruct_FlythroughDesc_Shot> shots;
	std::vector<Substruct_FlythroughDesc_Scene> scenes;
	uint16_t shotCount;
	uint16_t sceneCount;
	uint16_t unkCount;

	class Substruct_FlythroughDesc_Unknown : public PacketSubstruct {
	public:
		Substruct_FlythroughDesc_Unknown(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		~Substruct_FlythroughDesc_Unknown() = default;

		struct Substruct_Int : public PacketSubstruct {
			Substruct_Int(uint32_t ver = 0) : PacketSubstruct(ver, true) {

			}

			void RegisterElements() override {
				RegisterInt32(myInt);
			}

			int32_t myInt;
		};

		int32_t unknown;
		std::vector<Substruct_Int> ints;
		uint16_t count;

		void RegisterElements() override {
			RegisterInt32(unknown);
			auto e = RegisterUInt16(count);
			e->SetMyArray(RegisterArray(ints, Substruct_Int));
		}
	};

	std::vector<Substruct_FlythroughDesc_Unknown> unkArray;

	void RegisterElements() override {
		Register16String(unkString1);
		Register16String(unkString2);
		RegisterUInt64(unknown1);
		RescopeArrayElement(unknown2);
		RegisterFloat(unknown2)->SetCount(5);
		RegisterUInt8(unknown3);
		RegisterInt32(unknown4);
		RescopeArrayElement(unknown5);
		RegisterUInt8(unknown5)->SetCount(5);
		auto e = RegisterBool(unknown6);
		auto vec = RegisterFloat(unknownVec.x);
		vec->SetCount(3);
		vec->SetIsVariableSet(e);
		auto asize = RegisterUInt16(shotCount);
		asize->SetMyArray(RegisterArray(shots, Substruct_FlythroughDesc_Shot));
		asize = RegisterUInt16(sceneCount);
		asize->SetMyArray(RegisterArray(scenes, Substruct_FlythroughDesc_Scene));
		auto unkSize = RegisterUInt16(unkCount);
		unkSize->SetMyArray(RegisterArray(unkArray, Substruct_FlythroughDesc_Unknown));
	}
};

class OP_PlayFlythroughCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_PlayFlythroughCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver), flythrough(ver), bHasFlythrough(true) {
		RegisterElements();
	}

	~OP_PlayFlythroughCmd_Packet() = default;

	Substruct_FlythroughDesc flythrough;
	int32_t unknown2;
	bool bHasFlythrough;
	uint8_t unknown1;

	void RegisterElements() {
		RegisterUInt8(unknown1);
		RegisterInt32(unknown2);
		auto e = RegisterBool(bHasFlythrough);
		RegisterSubstruct(flythrough)->SetIsVariableSet(e);
	}
};