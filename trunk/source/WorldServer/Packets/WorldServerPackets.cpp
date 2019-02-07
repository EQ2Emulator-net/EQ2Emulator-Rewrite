#include "stdafx.h"

#include "OP_AllCharactersDescReplyMsg_Packet.h"
#include "OP_AllCharactersDescRequestMsg_Packet.h"
#include "OP_AllWSDescRequestMsg_Packet.h"
#include "OP_LoginRequestMsg_Packet.h"
#include "OP_WorldListMsg_Packet.h"
#include "OP_DeleteCharacterRequestMsg_Packet.h"
#include "OP_DeleteCharacterReplyMsg_Packet.h"

// Server -> Client
RegisterLoginStruct("OP_AllCharactersDescReplyMsg", OP_AllCharactersDescReplyMsg_Packet, 1, 1096);
RegisterLoginStruct("OP_AllWSDescRequestMsg", OP_AllWSDescRequestMsg_Packet, 1);
RegisterLoginStruct("OP_WorldListMsg", OP_WorldListMsg_Packet, 1, 60101);
RegisterLoginStruct("OP_DeleteCharacterReplyMsg", OP_DeleteCharacterReplyMsg_Packet, 1);

// Client -> Server
RegisterLoginStruct("OP_AllCharactersDescRequestMsg", OP_AllCharactersDescRequestMsg_Packet, 1);
RegisterLoginStruct("OP_LoginRequestMsg", OP_LoginRequestMsg_Packet, 1, 1212);
RegisterLoginStruct("OP_DeleteCharacterRequestMsg", OP_DeleteCharacterRequestMsg_Packet, 1);