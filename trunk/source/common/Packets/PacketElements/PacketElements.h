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
#define PushElementForRegistration(e, n) (elements.push_back(e), elements.back()->SetName(n), elements.back())
#define Register8String(e) PushElementForRegistration(new Packet8String(e), #e) 
#define Register16String(e) PushElementForRegistration(new Packet16String(e), #e)  
#define Register32String(e) PushElementForRegistration(new Packet32String(e), #e) 
#define RegisterUInt8(e) PushElementForRegistration(new PacketUInt8(e), #e)  
#define RegisterUInt16(e) PushElementForRegistration(new PacketUInt16(e), #e) 
#define RegisterUInt32(e) PushElementForRegistration(new PacketUInt32(e), #e)  
#define RegisterUInt64(e) PushElementForRegistration(new PacketUInt64(e), #e) 
#define RegisterInt8(e) PushElementForRegistration(new PacketInt8(e), #e) 
#define RegisterInt16(e) PushElementForRegistration(new PacketInt16(e), #e) 
#define RegisterInt32(e) PushElementForRegistration(new PacketInt32(e), #e)  
#define RegisterInt64(e) PushElementForRegistration(new PacketInt64(e), #e)  
#define RegisterSubstruct(e) PushElementForRegistration(new PacketSubstructParent(e), #e)
#define RegisterArray(e, t) PushElementForRegistration(new PacketArray<t>(e), #e)