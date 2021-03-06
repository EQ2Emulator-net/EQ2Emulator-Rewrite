#include "stdafx.h"

#include "OP_KeyRequest_Packet.h"
#include "OP_LoginReplyMsg_Packet.h"
#include "OP_UIEvent_Packet.h"

RegisterCommonStruct("OP_LoginRequestMsg", OP_KeyRequest_Packet, 1);
RegisterCommonStruct("OP_LoginReplyMsg", OP_LoginReplyMsg_Packet, 283, 284, 864, 865, 1096, 1188, 57080, 60100, 67650);
RegisterCommonStruct("OP_UIEvent", OP_UIEvent_Packet, 1, 1209);