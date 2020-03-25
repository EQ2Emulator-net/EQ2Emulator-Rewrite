#include "stdafx.h"

#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "OP_LoginByNumRequestMsg_Packet.h"
#include "OP_ZoneInfoMsg_Packet.h"
#include "OP_DoneLoadingZoneResourcesMsg_Packet.h"
#include "OP_DoneSendingInitialEntitiesMsg_Packet.h"
#include "OP_DoneLoadingEntityResourcesMsg_Packet.h"
#include "OP_EqSetPOVGhostCmd_Packet.h"
#include "OP_EqSetControlGhostCmd_Packet.h"
#include "OP_CreateGhostCmd_Packet.h"
#include "OP_UpdateCharacterSheetMsg_Packet.h"
#include "OP_DefaultGroupOptionsRequestMsg_Packet.h"
#include "OP_DefaultGroupOptionsMsg_Packet.h"
#include "OP_MoveableObjectPlacementCriteri_Packet.h"
#include "OP_GameWorldTimeMsg_Packet.h"
#include "OP_PredictionUpdateMsg.h"
#include "OP_UpdatePositionMsg.h"
#include "OP_SetRemoteCmdsMsg_Packet.h"
#include "OP_RemoteCmdMsg_Packet.h"

// Server to Client
RegisterZoneStruct("OP_ZoneInfoMsg", OP_ZoneInfoMsg_Packet, 1, 942, 957, 959, 1067, 1096, 1142, 1193, 63119, 63182);
RegisterZoneStruct("OP_DoneSendingInitialEntitiesMsg", OP_DoneSendingInitialEntitiesMsg_Packet, 1);
RegisterZoneStruct("OP_ClientCmdMsg", OP_ClientCmdMsg_Packet, 1);
RegisterZoneStruct("OP_EqSetPOVGhostCmd", OP_EqSetPOVGhostCmd_Packet, 1);
RegisterZoneStruct("OP_EqSetControlGhostCmd", OP_EqSetControlGhostCmd_Packet, 1, 1096, 1119, 1193);
RegisterZoneStruct("OP_EqCreateGhostCmd", OP_CreateGhostCmd_Packet, 1, 860, 864, 927, 936, 
	954, 955, 996, 1096, 1119, 1142, 1161, 1188, 1198, 57080, 68618, 60055, 67633);
RegisterZoneStruct("OP_UpdateCharacterSheetMsg", OP_UpdateCharacterSheetMsg_Packet, 1);
RegisterZoneStruct("OP_DefaultGroupOptionsMsg", OP_DefaultGroupOptionsMsg_Packet, 1);
RegisterZoneStruct("OP_MoveableObjectPlacementCriteri", OP_MoveableObjectPlacementCriteri_Packet, 1);
RegisterZoneStruct("OP_GameWorldTimeMsg", OP_GameWorldTimeMsg_Packet, 1, 1193);
RegisterZoneStruct("OP_SetRemoteCmdsMsg", OP_SetRemoteCmdsMsg_Packet, 1);

// Client to Server
RegisterZoneStruct("OP_LoginByNumRequestMsg", OP_LoginByNumRequestMsg_Packet, 1, 1212);
RegisterZoneStruct("OP_DoneLoadingZoneResourcesMsg", OP_DoneLoadingZoneResourcesMsg_Packet, 1);
RegisterZoneStruct("OP_DoneLoadingEntityResourcesMsg", OP_DoneLoadingEntityResourcesMsg_Packet, 1);
RegisterZoneStruct("OP_DefaultGroupOptionsRequestMsg", OP_DefaultGroupOptionsRequestMsg_Packet, 1);
RegisterZoneStruct("OP_UpdatePositionMsg", OP_UpdatePositionMsg, 1, 1096, 1144);
RegisterZoneStruct("OP_RemoteCmdMsg", OP_RemoteCmdMsg_Packet, 1);


//Bidirectional
RegisterZoneStruct("OP_PredictionUpdateMsg", OP_PredictionUpdateMsg_Packet, 1);