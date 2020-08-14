#include "stdafx.h"

#include "OP_AllCharactersDescReplyMsg_Packet.h"
#include "OP_AllCharactersDescRequestMsg_Packet.h"
#include "OP_AllWSDescRequestMsg_Packet.h"
#include "OP_LoginRequestMsg_Packet.h"
#include "OP_WorldListMsg_Packet.h"
#include "OP_DeleteCharacterRequestMsg_Packet.h"
#include "OP_DeleteCharacterReplyMsg_Packet.h"
#include "OP_PlayCharacterRequestMsg_Packet.h"
#include "OP_LsClientBaselogReplyMsg_Packet.h"
#include "OP_LsClientCrashlogReplyMsg_Packet.h"
#include "OP_LsClientAlertlogReplyMsg_Packet.h"
#include "OP_LsClientVerifylogReplyMsg_Packet.h"
#include "OP_CreateCharacterRequestMsg_Packet.h"
#include "OP_CreateCharacterReplyMsg_Packet.h"
#include "../../common/Packets/EQ2Packets/OP_LoginReplyMsg_Packet.h"

// Server -> Client
RegisterLoginStruct("OP_AllCharactersDescReplyMsg", OP_AllCharactersDescReplyMsg_Packet, 1, 1096, 67650);
RegisterLoginStruct("OP_AllWSDescRequestMsg", OP_AllWSDescRequestMsg_Packet, 1);
RegisterLoginStruct("OP_WorldListMsg", OP_WorldListMsg_Packet, 1, 60100);
RegisterLoginStruct("OP_DeleteCharacterReplyMsg", OP_DeleteCharacterReplyMsg_Packet, 283, 284);
RegisterLoginStruct("OP_PlayCharacterReplyMsg", OP_PlayCharacterReplyMsg_Packet, 1, 1096, 60099);
RegisterLoginStruct("OP_CreateCharacterReplyMsg", OP_CreateCharacterReplyMsg_Packet, 1);

// Client -> Server
RegisterLoginStruct("OP_AllCharactersDescRequestMsg", OP_AllCharactersDescRequestMsg_Packet, 1);
RegisterLoginStruct("OP_LoginRequestMsg", OP_LoginRequestMsg_Packet, 283, 284, 1212);
RegisterLoginStruct("OP_DeleteCharacterRequestMsg", OP_DeleteCharacterRequestMsg_Packet, 1);
RegisterLoginStruct("OP_PlayCharacterRequestMsg", OP_PlayCharacterRequestMsg_Packet, 283, 284);
RegisterLoginStruct("OP_LsClientBaselogReplyMsg", OP_LsClientBaselogReplyMsg_Packet, 1);
RegisterLoginStruct("OP_LsClientCrashlogReplyMsg", OP_LsClientCrashlogReplyMsg_Packet, 1);
RegisterLoginStruct("OP_LsClientAlertlogReplyMsg", OP_LsClientAlertlogReplyMsg_Packet, 1);
RegisterLoginStruct("OP_LsClientVerifylogReplyMsg", OP_LsClientVerifylogReplyMsg_Packet, 1);
RegisterLoginStruct("OP_CreateCharacterRequestMsg", OP_CreateCharacterRequestMsg_Packet, 283, 284, 869, 57080, 67650);