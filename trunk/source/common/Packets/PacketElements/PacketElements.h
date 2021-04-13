#pragma once

#include <string>
#include <stdint.h>

// Strings
#include "Packet8String.h"
#include "Packet16String.h"
#include "Packet32String .h"
#include "PacketChar.h"
#include "PacketCharString.h"

// Integers
#include "PacketInt8.h"
#include "PacketUInt8.h"
#include "PacketInt16.h"
#include "PacketUInt16.h"
#include "PacketInt32.h"
#include "PacketUInt32.h"
#include "PacketInt64.h"
#include "PacketUInt64.h"

// Floats
#include "PacketFloat.h"
#include "PacketDouble.h"

// Color
#include "PacketEQ2Color.h"
#include "PacketEQ2ColorFloat.h"

// Misc
#include "PacketArray.h"
#include "PacketSubstruct.h"
#include "PacketEQ2EquipmentItem.h"
#include "PacketOversizedByte.h"
#include "PacketBool.h"

//Register macros, these are intended to be used from RegisterElements()
#define PushElementForRegistration(e, n, t) (elements.push_back(e), elements.back()->SetName(n), static_cast<t*>(elements.back()))
#define Register8String(e) PushElementForRegistration(new Packet8String(e), #e, Packet8String)
#define Register16String(e) PushElementForRegistration(new Packet16String(e), #e, Packet16String)
#define Register32String(e) PushElementForRegistration(new Packet32String(e), #e, Packet32String)
#define RegisterChar(e) PushElementForRegistration(new PacketChar(e), #e, PacketChar)
#define RegisterUInt8(e) PushElementForRegistration(new PacketUInt8(e), #e, PacketUInt8)
#define RegisterUInt16(e) PushElementForRegistration(new PacketUInt16(e), #e, PacketUInt16)
#define RegisterUInt32(e) PushElementForRegistration(new PacketUInt32(e), #e, PacketUInt32)
#define RegisterUInt64(e) PushElementForRegistration(new PacketUInt64(e), #e, PacketUInt64)
#define RegisterInt8(e) PushElementForRegistration(new PacketInt8(e), #e, PacketInt8)
#define RegisterInt16(e) PushElementForRegistration(new PacketInt16(e), #e, PacketInt16)
#define RegisterInt32(e) PushElementForRegistration(new PacketInt32(e), #e, PacketInt32)
#define RegisterInt64(e) PushElementForRegistration(new PacketInt64(e), #e, PacketInt64)
#define RegisterSubstruct(e) PushElementForRegistration(new PacketSubstructParent<std::remove_reference_t<decltype(e)>>(e), #e, PacketSubstructParent<std::remove_reference_t<decltype(e)>>)
#define RegisterArray(e, t) (static_cast<PacketArray<t>*>(PushElementForRegistration(new PacketArray<t>(e), #e, PacketArrayBase))->SetVersion(GetVersion()), static_cast<PacketArrayBase*>(elements.back()))
#define ELE_REGISTER_COMMA ,
#define RegisterElementArray(e, t, p) PushElementForRegistration(new PacketElementArray<t ELE_REGISTER_COMMA p>(e), #e, PacketElementArrayBase)
#define RegisterEQ2Color(e) PushElementForRegistration(new PacketEQ2Color(e), #e, PacketEQ2Color)
#define RegisterEQ2ColorFloat(e) PushElementForRegistration(new PacketEQ2ColorFloat(e), #e, PacketEQ2ColorFloat)
#define RegisterEQ2EquipmentItem(e, bShortType) PushElementForRegistration(new PacketEQ2EquipmentItem(e, (bShortType)), #e, PacketEQ2EquipmentItem)
#define RegisterOversizedByte(e) PushElementForRegistration(new PacketOversizedByte(e), #e, PacketOversizedByte)
#define RegisterFloat(e) PushElementForRegistration(new PacketFloat(e), #e, PacketFloat)
#define RegisterBool(e) PushElementForRegistration(new PacketBool(e), #e, PacketBool);
#define RegisterDouble(e) PushElementForRegistration(new PacketDouble(e), #e, PacketDouble);
#define RegisterCharString(e, n) PushElementForRegistration(new PacketCharString(e, (n)), #e, PacketCharString);
#define RescopeArrayElement(e) auto& e = reinterpret_cast<decltype(this->e[0])&>(this->e[0]);
#define RescopeToReference(e, t) auto& e = reinterpret_cast<t&>(this->e)