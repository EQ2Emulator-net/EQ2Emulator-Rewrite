#include "stdafx.h"

#include "OP_KeyRequest_Packet.h"
#include "OP_LoginReplyMsg_Packet.h"

RegisterWorldStruct("OP_WSLoginRequestMsg", OP_KeyRequest_Packet, 1);
RegisterWorldStruct("OP_LoginReplyMsg", OP_LoginReplyMsg_Packet, 1, 1096, 1188);