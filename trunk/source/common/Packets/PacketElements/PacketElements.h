#pragma once

#include <string>
#include <stdint.h>

// Strings
#include "Packet8String.h"
#include "Packet16String.h"
#include "Packet32String .h"

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

// Color
#include "PacketEQ2Color.h"

// Misc
#include "PacketArray.h"
#include "PacketSubstruct.h"

//Register macros, these are intended to be used from RegisterElements()
#define PushElementForRegistration(e, n, t) (elements.push_back(e), elements.back()->SetName(n), static_cast<t*>(elements.back()))
#define Register8String(e) PushElementForRegistration(new Packet8String(e), #e, Packet8String) 
#define Register16String(e) PushElementForRegistration(new Packet16String(e), #e, Packet16String)  
#define Register32String(e) PushElementForRegistration(new Packet32String(e), #e, Packet32String) 
#define RegisterUInt8(e) PushElementForRegistration(new PacketUInt8(e), #e, PacketUInt8)  
#define RegisterUInt16(e) PushElementForRegistration(new PacketUInt16(e), #e, PacketUInt16) 
#define RegisterUInt32(e) PushElementForRegistration(new PacketUInt32(e), #e, PacketUInt32)  
#define RegisterUInt64(e) PushElementForRegistration(new PacketUInt64(e), #e, PacketUInt64) 
#define RegisterInt8(e) PushElementForRegistration(new PacketInt8(e), #e, PacketInt8) 
#define RegisterInt16(e) PushElementForRegistration(new PacketInt16(e), #e, PacketInt16) 
#define RegisterInt32(e) PushElementForRegistration(new PacketInt32(e), #e, PacketInt32)  
#define RegisterInt64(e) PushElementForRegistration(new PacketInt64(e), #e, PacketInt64)
#define RegisterSubstruct(e) PushElementForRegistration(new PacketSubstructParent<decltype(e)>(e), #e, PacketSubstructParent)
#define RegisterArray(e, t) PushElementForRegistration(new PacketArray<t>(e), #e, PacketArrayBase)
#define RegisterEQ2Color(e) PushElementForRegistration(new PacketEQ2Color(e), #e, PacketEQ2Color)