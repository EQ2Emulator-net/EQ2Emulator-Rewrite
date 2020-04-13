#include "stdafx.h"

#include "OP_KeyRequest_Packet.h"
#include "OP_LoginReplyMsg_Packet.h"

RegisterCommonStruct("OP_LoginRequestMsg", OP_KeyRequest_Packet, 1);
RegisterCommonStruct("OP_LoginReplyMsg", OP_LoginReplyMsg_Packet, 283, 284, 1096, 1188);