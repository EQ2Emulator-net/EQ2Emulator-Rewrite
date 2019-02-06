#include "stdafx.h"

#include "OP_AllCharactersDescReplyMsg_Packet.h"
#include "OP_AllCharactersDescRequestMsg_Packet.h"
#include "OP_AllWSDescRequestMsg_Packet.h"
#include "OP_LoginRequestMsg_Packet.h"
#include "OP_WorldListMsg_Packet.h"

RegisterLoginStruct("OP_AllCharactersDescReplyMsg", OP_AllCharactersDescReplyMsg_Packet, 1, 1096);
RegisterLoginStruct("OP_AllCharactersDescRequestMsg", OP_AllCharactersDescRequestMsg_Packet, 1);
RegisterLoginStruct("OP_AllWSDescRequestMsg", OP_AllWSDescRequestMsg_Packet, 1);
RegisterLoginStruct("OP_LoginRequestMsg", OP_LoginRequestMsg_Packet, 1, 1212);
RegisterLoginStruct("OP_WorldListMsg", OP_WorldListMsg_Packet, 1, 60101);