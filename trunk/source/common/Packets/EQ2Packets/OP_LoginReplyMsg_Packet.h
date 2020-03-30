#pragma once

#include "../EQ2Packet.h"
#include "../PacketElements/PacketElements.h"
#include "OpcodeManager.h"

enum class LoginReply : uint8_t {
	EAccepted = 0,
	EBadPassword = 1,
	ECurrentlyPlaying = 2,
	EBadVersion = 6,
	EUnknown = 7
};

class OP_LoginReplyMsg_Packet : public EQ2Packet {
public:
	OP_LoginReplyMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		Response = 0;
		ParentalControlFlag = 0;
		ParentalControlTimer = 0;
		Unknown2 = 0;
		AccountID = 0;
		//Unknown3 = "";
		ResetAppearance = 0;
		DoNotForceSoga = 1;
		Unknown4 = 0;
		Unknown5 = 0;
		memset(Unknown6, 0, sizeof(Unknown6));
		Unknown7 = 0;
		Unknown7a = 0;
		RaceUnknown = 0;
		memset(Unknown8, 0, sizeof(Unknown8));
		memset(Unknown9, 0, sizeof(Unknown9));
		Unknown10 = 1;
		NumClassItems = 0;
		UnknownArraySize = 0;
		Unknown11 = 0;
		SubscriptionLevel = 2;
		RaceFlag = 0;
		ClassFlag = 0;
		//Password = "";
		//Username = "";
		//Service = "";

		// 60100 elements
		Unknown12 = 0;
		LVL90NumClassItems = 0;
		Unknown13 = 0;
		TimeLockedNumClassItems = 0;
		memset(Unknown14, 0, sizeof(Unknown14));

		RegisterElements();
	}

	uint8_t Response;
	std::string worldName;
	uint8_t ParentalControlFlag;
	uint64_t ParentalControlTimer;
	uint32_t Unknown2;					//<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
	uint32_t AccountID;
	std::string Unknown3;
	uint8_t ResetAppearance;
	uint8_t DoNotForceSoga;
	uint8_t Unknown4;
	uint16_t Unknown5;
	uint8_t Unknown6[5];					//<Data ElementName = "unknown6" Type = "int8" Size = "5" / >
	uint32_t Unknown7;
	uint32_t Unknown7a; // 1188 7a = uint32
	uint8_t RaceUnknown;
	uint8_t Unknown8[3];					// size = 1 in 60100+ <Data ElementName = "unknown8" Type = "int8" Size = "3" / > < !--possibly related to rave_unknown but can't confirm-->
	uint8_t Unknown9[3];
	uint8_t Unknown10;
	uint8_t NumClassItems;				//<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
	struct ClassItem : public PacketSubstruct {
		uint8_t ClassID;
		uint8_t NumItems;
		struct StartingItem : public PacketSubstruct {
			uint32_t ModelID;
			uint8_t SlotID;
			uint8_t UseColor;
			uint8_t UseHighlightColor;
			EQ2Color ModelColor;
			EQ2Color ModelHighlightColor;

			StartingItem() : PacketSubstruct(0) {
				ModelID = 0;
				SlotID = 0;
				UseColor = 1;
				UseHighlightColor = 1;
				ModelColor.Red = 255;
				ModelColor.Green = 255;
				ModelColor.Blue = 255;
				ModelHighlightColor.Red = 255;
				ModelHighlightColor.Green = 255;
				ModelHighlightColor.Blue = 255;

				RegisterElements();
			}

			void RegisterElements() {
				if (GetVersion() >= 57080)
					RegisterUInt32(ModelID);
				else {
					RescopeToReference(ModelID, uint16_t);
					RegisterUInt16(ModelID);
				}
				RegisterUInt8(SlotID);
				RegisterUInt8(UseColor);
				RegisterUInt8(UseHighlightColor);
				RegisterEQ2Color(ModelColor);
				RegisterEQ2Color(ModelHighlightColor);
			}
		};
		std::vector<StartingItem> StartingItems;

		ClassItem() : PacketSubstruct(0) {
			ClassID = 0;
			NumItems = 0;

			RegisterElements();
		}

		void RegisterElements() {
			RegisterUInt8(ClassID);
			PacketUInt8* asize = RegisterUInt8(NumItems);
			asize->SetMyArray(RegisterArray(StartingItems, StartingItem));
		}
	};
	std::vector<ClassItem> ClassItems;
	uint8_t UnknownArraySize;
	struct UnknownArray : public PacketSubstruct {
		uint32_t Array2Unknown;

		UnknownArray() : PacketSubstruct(0) {
			Array2Unknown = 0;

			RegisterElements();
		}

		void RegisterElements() {
			RegisterUInt32(Array2Unknown);
		}
	};
	std::vector<UnknownArray> UnknownArray2;
	uint32_t Unknown11;
	uint32_t SubscriptionLevel;
	uint32_t RaceFlag;
	uint32_t ClassFlag;
	std::string Password;
	std::string Username;
	std::string Service; //1188

	uint8_t Unknown12;
	uint8_t LVL90NumClassItems;
	std::vector<ClassItem> LVL90ClassItems;

	uint8_t Unknown13;
	uint8_t TimeLockedNumClassItems;
	std::vector<ClassItem> TimeLockedClassItems;

	uint8_t Unknown14[13]; // size = 13

private:
	void RegisterElements() {
		RegisterUInt8(Response);
		Register16String(worldName);
		RegisterUInt8(ParentalControlFlag);
		RegisterUInt64(ParentalControlTimer);
		if (GetVersion() > 283) {
			RegisterUInt32(Unknown2);
		}
		RegisterUInt32(AccountID);
		if (GetVersion() <= 283) {
			return;
		}
		Register16String(Unknown3);
		RegisterUInt8(ResetAppearance);
		RegisterUInt8(DoNotForceSoga);
		RegisterUInt8(Unknown4);
		RegisterUInt16(Unknown5);
		RescopeArrayElement(Unknown6);
		RegisterUInt8(Unknown6)->SetCount(5);
		RegisterUInt32(Unknown7);
		
		RegisterUInt32(Unknown7a);

		RegisterUInt8(RaceUnknown);

		RescopeArrayElement(Unknown8);
		if (GetVersion() >= 60100)
			RegisterUInt8(Unknown8);
		else
			RegisterUInt8(Unknown8)->SetCount(3);

		RescopeArrayElement(Unknown9);
		if (GetVersion() >= 60100)
			RegisterUInt8(Unknown9)->SetCount(3);
		else
			RegisterUInt8(Unknown9);

		RegisterUInt8(Unknown10);
		// If Unknown10 is set include the following
		PacketUInt8* asize = RegisterUInt8(NumClassItems);
		asize->SetMyArray(RegisterArray(ClassItems, ClassItem));
		// end of if var set

		asize = RegisterUInt8(UnknownArraySize);
		asize->SetMyArray(RegisterArray(UnknownArray2, UnknownArray));
		RegisterUInt32(Unknown11);
		RegisterUInt32(SubscriptionLevel);
		RegisterUInt32(RaceFlag);
		RegisterUInt32(ClassFlag);
		Register16String(Password);
		Register16String(Username);
		if (GetVersion() >= 1188)
			Register16String(Service); // service

		if (GetVersion() >= 60100) {
			RegisterUInt8(Unknown12);
			// if Unknown12 is set
			asize = RegisterUInt8(LVL90NumClassItems);
			asize->SetMyArray(RegisterArray(LVL90ClassItems, ClassItem));

			RegisterUInt8(Unknown13);
			// if Unknown13 is set
			asize = RegisterUInt8(TimeLockedNumClassItems);
			asize->SetMyArray(RegisterArray(TimeLockedClassItems, ClassItem));

			RescopeArrayElement(Unknown14);
			RegisterUInt8(Unknown14)->SetCount(13);
		}
	}
};

/*
<Struct Name = "LS_LoginReplyMsg" ClientVersion = "1" OpcodeName = "OP_LoginReplyMsg">
<Data ElementName = "login_response" Type = "int8" Size = "1" / >
<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" / >
<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" / >
<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" / >
<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
<Data ElementName = "account_id" Type = "int32" Size = "1" / >
<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" / >
<Data ElementName = "reset_appearance" Type = "int8" Size = "1" / >
<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" / >
<Data ElementName = "unknown5" Type = "int16" Size = "1" / >
<Data ElementName = "unknown6" Type = "int8" Size = "1" / >
<Data ElementName = "unknown7" Type = "int32" Size = "1" / >
<Data ElementName = "unknown8" Type = "int8" Size = "2" / >
<Data ElementName = "unknown10" Type = "int8" Size = "1" / >
<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
		<Data ElementName = "model_id" Type = "int16" / >
		<Data ElementName = "slot_id" Type = "int8" / >
		<Data ElementName = "use_color" Type = "int8" / >
		<Data ElementName = "use_highlight_color" Type = "int8" / >
		<Data ElementName = "model_color" Type = "EQ2_Color" / >
		<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
< / Data>
<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" / >
<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size" >
	<Data ElementName = "array2_unknown" Type = "int32" Size = "1" / >
< / Data>
< / Struct>

<Struct Name = "LS_LoginReplyMsg" ClientVersion = "843" OpcodeName = "OP_LoginReplyMsg">
<Data ElementName = "login_response" Type = "int8" Size = "1" / >
<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" / >
<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" / >
<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" / >
<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
<Data ElementName = "account_id" Type = "int32" Size = "1" / >
<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" / >
<Data ElementName = "reset_appearance" Type = "int8" Size = "1" / >
<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" / >
<Data ElementName = "unknown4" Type = "int8" Size = "1" / >
<Data ElementName = "unknown5" Type = "int16" Size = "1" / >
<Data ElementName = "unknown6" Type = "int8" Size = "1" / >
<Data ElementName = "unknown7" Type = "int32" Size = "1" / >
<Data ElementName = "race_unknown" Type = "int8" Size = "1" / >
<Data ElementName = "unknown8" Type = "int8" Size = "3" / >
<Data ElementName = "unknown9" Type = "int8" Size = "1" / >
<Data ElementName = "unknown10" Type = "int8" Size = "1" / >
<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
		<Data ElementName = "model_id" Type = "int16" / >
		<Data ElementName = "slot_id" Type = "int8" / >
		<Data ElementName = "use_color" Type = "int8" / >
		<Data ElementName = "use_highlight_color" Type = "int8" / >
		<Data ElementName = "model_color" Type = "EQ2_Color" / >
		<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
< / Data>
<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" / >
<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size" >
	<Data ElementName = "array2_unknown" Type = "int32" Size = "1" / >
< / Data>
< / Struct>

<Struct Name = "LS_LoginReplyMsg" ClientVersion = "1096" OpcodeName = "OP_LoginReplyMsg">
<Data ElementName = "login_response" Type = "int8" Size = "1" / >
<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" / > <!--possibly parental control password-->
<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" / >
<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" / >
<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
<Data ElementName = "account_id" Type = "int32" Size = "1" / >
<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" / > <!--assume this is a string, could be an array though-->
<Data ElementName = "reset_appearance" Type = "int8" Size = "1" / >
<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" / >
<Data ElementName = "unknown4" Type = "int8" Size = "1" / >
<Data ElementName = "unknown5" Type = "int16" Size = "1" / >
<Data ElementName = "unknown6" Type = "int8" Size = "5" / >
<Data ElementName = "unknown7" Type = "int32" Size = "1" / > <!--expansion info maybe-->
<Data ElementName = "race_unknown" Type = "int8" Size = "1" / >
<Data ElementName = "unknown8" Type = "int8" Size = "3" / > < !--possibly related to rave_unknown but can't confirm-->
<Data ElementName = "unknown9" Type = "int8" Size = "1" / > <!--screws with models, possibly related to that skeletal revamp that was never finished-->
<Data ElementName = "unknown10" Type = "int8" Size = "1" / >
<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
		<Data ElementName = "model_id" Type = "int16" / >
		<Data ElementName = "slot_id" Type = "int8" / >
		<Data ElementName = "use_color" Type = "int8" / >
		<Data ElementName = "use_highlight_color" Type = "int8" / >
		<Data ElementName = "model_color" Type = "EQ2_Color" / >
		<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
< / Data>
<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" / >
<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size" >
	<Data ElementName = "array2_unknown" Type = "int32" Size = "1" / >
< / Data>
<Data ElementName = "unknown11" Type = "int32" Size = "1" / >
<Data ElementName = "sub_level" Type = "int32" Size = "1" / >
<Data ElementName = "race_flag" Type = "int32" Size = "1" / >
<Data ElementName = "class_flag" Type = "int32" Size = "1" / >
<!--Both of the following are encrypted, password is correct guessed that the second is username, neither need to be set-->
<Data ElementName = "password" Type = "EQ2_16Bit_String" Size = "1" / >
<Data ElementName = "username" Type = "EQ2_16bit_String" Size = "1" / >
< / Struct>


	<Struct Name = "LS_LoginReplyMsg" ClientVersion = "1142" OpcodeName = "OP_LoginReplyMsg">
	<Data ElementName = "login_response" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" / >
	<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
	<Data ElementName = "account_id" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "reset_appearance" Type = "int8" Size = "1" / >
	<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown4" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown5" Type = "int16" Size = "1" / >
	<Data ElementName = "unknown6" Type = "int8" Size = "5" / >
	<Data ElementName = "unknown7" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown7a" Type = "int32" Size = "1" / >
	<Data ElementName = "race_unknown" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown8" Type = "int8" Size = "3" / >
	<Data ElementName = "unknown9" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown10" Type = "int8" Size = "1" / >
	<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
	<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
	<Data ElementName = "model_id" Type = "int16" / >
	<Data ElementName = "slot_id" Type = "int8" / >
	<Data ElementName = "use_color" Type = "int8" / >
	<Data ElementName = "use_highlight_color" Type = "int8" / >
	<Data ElementName = "model_color" Type = "EQ2_Color" / >
	<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
	< / Data>
	<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size" >
	<Data ElementName = "array2_unknown" Type = "int32" Size = "1" / >
	< / Data>
	<Data ElementName = "unknown11" Type = "int32" Size = "1" / >
	<Data ElementName = "sub_level" Type = "int32" Size = "1" / >
	<Data ElementName = "race_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "class_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "password" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "username" Type = "EQ2_16bit_String" Size = "1" / >
	< / Struct>
	<Struct Name = "LS_LoginReplyMsg" ClientVersion = "1188" OpcodeName = "OP_LoginReplyMsg">
	<Data ElementName = "login_response" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" / >
	<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
	<Data ElementName = "account_id" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "reset_appearance" Type = "int8" Size = "1" / >
	<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown4" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown5" Type = "int16" Size = "1" / >
	<Data ElementName = "unknown6" Type = "int8" Size = "5" / >
	<Data ElementName = "unknown7" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown7a" Type = "int32" Size = "1" / >
	<Data ElementName = "race_unknown" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown8" Type = "int8" Size = "3" / >
	<Data ElementName = "unknown9" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown10" Type = "int8" Size = "1" / >
	<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
	<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
	<Data ElementName = "model_id" Type = "int16" / >
	<Data ElementName = "slot_id" Type = "int8" / >
	<Data ElementName = "use_color" Type = "int8" / >
	<Data ElementName = "use_highlight_color" Type = "int8" / >
	<Data ElementName = "model_color" Type = "EQ2_Color" / >
	<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
	< / Data>
	<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size" >
	<Data ElementName = "array2_unknown" Type = "int32" Size = "1" / >
	< / Data>
	<Data ElementName = "unknown11" Type = "int32" Size = "1" / >
	<Data ElementName = "sub_level" Type = "int32" Size = "1" / >
	<Data ElementName = "race_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "class_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "password" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "username" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "unknown12" Type = "EQ2_16bit_String" Size = "1" / >
	< / Struct>
	<Struct Name = "LS_LoginReplyMsg" ClientVersion = "57080" OpcodeName = "OP_LoginReplyMsg">
	<Data ElementName = "login_response" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" / >
	<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
	<Data ElementName = "account_id" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "reset_appearance" Type = "int8" Size = "1" / >
	<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown4" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown5" Type = "int16" Size = "1" / >
	<Data ElementName = "unknown6" Type = "int8" Size = "5" / >
	<Data ElementName = "unknown7" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown7a" Type = "int32" Size = "1" / >
	<Data ElementName = "race_unknown" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown8" Type = "int8" Size = "3" / >
	<Data ElementName = "unknown9" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown10" Type = "int8" Size = "1" / >
	<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
	<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
	<Data ElementName = "model_id" Type = "int32" / >
	<Data ElementName = "slot_id" Type = "int8" / >
	<Data ElementName = "use_color" Type = "int8" / >
	<Data ElementName = "use_highlight_color" Type = "int8" / >
	<Data ElementName = "model_color" Type = "EQ2_Color" / >
	<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
	< / Data>
	<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size" >
	<Data ElementName = "array2_unknown" Type = "int32" Size = "1" / >
	< / Data>
	<Data ElementName = "unknown11" Type = "int32" Size = "1" / >
	<Data ElementName = "sub_level" Type = "int32" Size = "1" / >
	<Data ElementName = "race_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "class_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "password" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "username" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "service" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web1" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web2" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web3" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web4" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web5" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web6" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web7" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web8" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "web9" Type = "EQ2_16bit_String" Size = "1" / >
	<!--Seems to be a repeat of the first array-->
	<Data ElementName = "unknown12" Type = "int8" Size = "1" / >
	<Data ElementName = "lvl90_num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
	<Data ElementName = "lvl90_class_items" Type = "Array" ArraySizeVariable = "lvl90_num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
	<Data ElementName = "model_id" Type = "int32" / >
	<Data ElementName = "slot_id" Type = "int8" / >
	<Data ElementName = "use_color" Type = "int8" / >
	<Data ElementName = "use_highlight_color" Type = "int8" / >
	<Data ElementName = "model_color" Type = "EQ2_Color" / >
	<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
	< / Data>
	<Data ElementName = "unknown13" Type = "int8" Size = "5" / >
	< / Struct>
<Struct Name = "LS_LoginReplyMsg" ClientVersion = "60100" OpcodeName = "OP_LoginReplyMsg">
	<Data ElementName = "login_response" Type = "int8" Size = "1" />
	<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" />
	<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" />
	<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" />
	<Data ElementName = "unknown2" Type = "int8" Size = "8" />
	<Data ElementName = "account_id" Type = "int32" Size = "1" />
	<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" />
	<Data ElementName = "reset_appearance" Type = "int8" Size = "1" />
	<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" />
	<Data ElementName = "unknown4" Type = "int8" Size = "1" />
	<Data ElementName = "unknown5" Type = "int16" Size = "1" />
	<Data ElementName = "unknown6" Type = "int8" Size = "5" />
	<Data ElementName = "unknown7" Type = "int32" Size = "1" />
	<Data ElementName = "unknown7a" Type = "int32" Size = "1" />
	<Data ElementName = "race_unknown" Type = "int8" Size = "1" />
	<Data ElementName = "unknown8" Type = "int8" Size = "1" />
	<Data ElementName = "unknown9" Type = "int8" Size = "3" />
	<Data ElementName = "unknown10" Type = "int8" Size = "1" />
	<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" />
	<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10">
		<Data ElementName = "class_id" Type = "int8" Size = "1" />
		<Data ElementName = "num_items" Type = "int8" Size = "1" />
		<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items">
			<Data ElementName = "model_id" Type = "int32" />
			<Data ElementName = "slot_id" Type = "int8" />
			<Data ElementName = "use_color" Type = "int8" />
			<Data ElementName = "use_highlight_color" Type = "int8" />
			<Data ElementName = "model_color" Type = "EQ2_Color" />
			<Data ElementName = "model_highlight_color" Type = "EQ2_Color" />
		</Data>
	</Data>
	<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" />
	<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size">
		<Data ElementName = "array2_unknown" Type = "int32" Size = "1" />
	</Data>
	<Data ElementName = "unknown11" Type = "int32" Size = "1" />
	<Data ElementName = "sub_level" Type = "int32" Size = "1" />
	<Data ElementName = "race_flag" Type = "int32" Size = "1" />
	<Data ElementName = "class_flag" Type = "int32" Size = "1" />
	<Data ElementName = "password" Type = "EQ2_16Bit_String" Size = "1" />
	<Data ElementName = "username" Type = "EQ2_16bit_String" Size = "1" />
	<Data ElementName = "service" Type = "EQ2_16bit_String" Size = "1" />
	<!--Seems to be a repeat of the first array-->
	<Data ElementName = "unknown12" Type = "int8" Size = "1" />
	<Data ElementName = "lvl90_num_class_items" Type = "int8" IfVariableSet = "unknown12" Size = "1" />
	<Data ElementName = "lvl90_class_items" Type = "Array" ArraySizeVariable = "lvl90_num_class_items" IfVariableSet = "unknown12">
		<Data ElementName = "class_id" Type = "int8" Size = "1" />
		<Data ElementName = "num_items" Type = "int8" Size = "1" />
		<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items">
			<Data ElementName = "model_id" Type = "int32" />
			<Data ElementName = "slot_id" Type = "int8" />
			<Data ElementName = "use_color" Type = "int8" />
			<Data ElementName = "use_highlight_color" Type = "int8" />
			<Data ElementName = "model_color" Type = "EQ2_Color" />
			<Data ElementName = "model_highlight_color" Type = "EQ2_Color" />
		</Data>
	</Data>
	<Data ElementName = "unknown13" Type = "int8" Size = "1" />
	<Data ElementName = "time_locked_num_class_items" Type = "int8" IfVariableSet = "unknown13" Size = "1" />
	<Data ElementName = "time_locked_class_items" Type = "Array" ArraySizeVariable = "time_locked_num_class_items" IfVariableSet = "unknown13">
		<Data ElementName = "class_id" Type = "int8" Size = "1" />
		<Data ElementName = "num_items" Type = "int8" Size = "1" />
		<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items">
			<Data ElementName = "model_id" Type = "int32" />
			<Data ElementName = "slot_id" Type = "int8" />
			<Data ElementName = "use_color" Type = "int8" />
			<Data ElementName = "use_highlight_color" Type = "int8" />
			<Data ElementName = "model_color" Type = "EQ2_Color" />
			<Data ElementName = "model_highlight_color" Type = "EQ2_Color" />
		</Data>
	</Data>
	<Data ElementName = "unknown14" Type = "int8" Size = "13" />
</Struct>
	<Struct Name = "LS_LoginReplyMsg" ClientVersion = "63181" OpcodeName = "OP_LoginReplyMsg">
	<Data ElementName = "login_response" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "parental_control_flag" Type = "int8" Size = "1" / >
	<Data ElementName = "parental_control_timer" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown2" Type = "int8" Size = "8" / >
	<Data ElementName = "account_id" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown3" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "reset_appearance" Type = "int8" Size = "1" / >
	<Data ElementName = "do_not_force_soga" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown4" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown5" Type = "int16" Size = "1" / >
	<Data ElementName = "unknown6" Type = "int8" Size = "5" / >
	<Data ElementName = "unknown6a" Type = "int8" Size = "8" / >
	<Data ElementName = "unknown7" Type = "int32" Size = "1" / >
	<Data ElementName = "unknown7a" Type = "int16" Size = "1" / >
	<Data ElementName = "race_unknown" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown8" Type = "int8" Size = "3" / >
	<Data ElementName = "unknown9" Type = "int8" Size = "3" / >
	<Data ElementName = "unknown10" Type = "int8" Size = "1" / >
	<Data ElementName = "num_class_items" Type = "int8" IfVariableSet = "unknown10" Size = "1" / >
	<Data ElementName = "class_items" Type = "Array" ArraySizeVariable = "num_class_items" IfVariableSet = "unknown10" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
	<Data ElementName = "model_id" Type = "int32" / >
	<Data ElementName = "slot_id" Type = "int8" / >
	<Data ElementName = "use_color" Type = "int8" / >
	<Data ElementName = "use_highlight_color" Type = "int8" / >
	<Data ElementName = "model_color" Type = "EQ2_Color" / >
	<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
	< / Data>
	<Data ElementName = "unknown_array2_size" Type = "int8" Size = "1" / >
	<Data ElementName = "unknown_array2" Type = "Array" ArraySizeVariable = "unknown_array2_size" >
	<Data ElementName = "array2_unknown" Type = "int32" Size = "1" / >
	< / Data>
	<Data ElementName = "unknown11" Type = "int32" Size = "1" / >
	<Data ElementName = "sub_level" Type = "int32" Size = "1" / >
	<Data ElementName = "race_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "class_flag" Type = "int32" Size = "1" / >
	<Data ElementName = "password" Type = "EQ2_16Bit_String" Size = "1" / >
	<Data ElementName = "username" Type = "EQ2_16bit_String" Size = "1" / >
	<Data ElementName = "service" Type = "EQ2_16bit_String" Size = "1" / >
	<!--Seems to be a repeat of the first array-->
	<Data ElementName = "unknown12" Type = "int8" Size = "1" / >
	<Data ElementName = "lvl90_num_class_items" Type = "int8" IfVariableSet = "unknown12" Size = "1" / >
	<Data ElementName = "lvl90_class_items" Type = "Array" ArraySizeVariable = "lvl90_num_class_items" IfVariableSet = "unknown12" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
	<Data ElementName = "model_id" Type = "int32" / >
	<Data ElementName = "slot_id" Type = "int8" / >
	<Data ElementName = "use_color" Type = "int8" / >
	<Data ElementName = "use_highlight_color" Type = "int8" / >
	<Data ElementName = "model_color" Type = "EQ2_Color" / >
	<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
	< / Data>
	<Data ElementName = "unknown13" Type = "int8" Size = "1" / >
	<Data ElementName = "time_locked_num_class_items" Type = "int8" IfVariableSet = "unknown13" Size = "1" / >
	<Data ElementName = "time_locked_class_items" Type = "Array" ArraySizeVariable = "time_locked_num_class_items" IfVariableSet = "unknown13" >
	<Data ElementName = "class_id" Type = "int8" Size = "1" / >
	<Data ElementName = "num_items" Type = "int8" Size = "1" / >
	<Data ElementName = "starting_items" Type = "Array" ArraySizeVariable = "num_items" >
	<Data ElementName = "model_id" Type = "int32" / >
	<Data ElementName = "slot_id" Type = "int8" / >
	<Data ElementName = "use_color" Type = "int8" / >
	<Data ElementName = "use_highlight_color" Type = "int8" / >
	<Data ElementName = "model_color" Type = "EQ2_Color" / >
	<Data ElementName = "model_highlight_color" Type = "EQ2_Color" / >
	< / Data>
	< / Data>
	<Data ElementName = "unknown14" Type = "int8" Size = "9" / >
	< / Struct>
		*/