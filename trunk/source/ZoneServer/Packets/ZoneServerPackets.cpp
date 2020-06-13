#include "stdafx.h"

#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "OP_LoginByNumRequestMsg_Packet.h"
#include "OP_ZoneInfoMsg_Packet.h"
#include "OP_DoneLoadingZoneResourcesMsg_Packet.h"
#include "OP_DoneSendingInitialEntitiesMsg_Packet.h"
#include "OP_DoneLoadingEntityResourcesMsg_Packet.h"
#include "OP_EqSetPOVGhostCmd_Packet.h"
#include "OP_EqSetControlGhostCmd_Packet.h"
#include "OP_UpdateCharacterSheetMsg_Packet.h"
#include "OP_DefaultGroupOptionsRequestMsg_Packet.h"
#include "OP_DefaultGroupOptionsMsg_Packet.h"
#include "OP_MoveableObjectPlacementCriteri_Packet.h"
#include "OP_GameWorldTimeMsg_Packet.h"
#include "OP_PredictionUpdateMsg.h"
#include "OP_UpdatePositionMsg.h"
#include "OP_SetRemoteCmdsMsg_Packet.h"
#include "OP_RemoteCmdMsg_Packet.h"
#include "OP_CreateSignWidgetCmd_Packet.h"
#include "OP_CreateWidgetCmd_Packet.h"
#include "OP_UpdateSpawnCmdMsg.h"
#include "OP_TeleportWithinZoneNoReloadMsg_Packet.h"
#include "OP_PaperdollImage_Packet.h"
#include "OP_UpdateTargetMsg.h"
#include "OP_ChangeZoneMsg_Packet.h"
#include "OP_ReadyToZoneMsg_Packet.h"
#include "OP_EqDestroyGhostCmd_Packet.h"
#include "OP_ChangeServerControlFlagMsg.h"
#include "OP_ChatFiltersMsg.h"
#include "OP_DisplayTextCmd_Packet.h"
#include "OP_HearChatCmd_Packet.h"
#include "OP_OnscreenMsgMsg_Packet.h"
#include "OP_DialogOpenCmd_Packet.h"
#include "OP_RequestCampMsg.h"
#include "OP_CampStartedMsg_Packet.h"
#include "OP_StoodMsg_Packet.h"
#include "OP_CampAbortedMsg_Packet.h"
#include "OP_BioUpdateMsg_Packet.h"
#include "OP_UpdateTitleCmd_Packet.h"
#include "OP_EqCannedEmoteCmd_Packet.h"
#include "OP_DialogCloseMsg_Packet.h"
#include "OP_DialogSelectMsg_Packet.h"
#include "OP_HearPlayFlavorCmd_Packet.h"
#include "OP_EntityVerbsVerbMsg_Packet.h"
#include "OP_MapFogDataUpdateMsg_Packet.h"
#include "OP_MapFogDataInitMsg_Packet.h"
#include "OP_MapRequest_Packet.h"
#include "OP_EqExamineInfoCmd.h"
#include "OP_UpdateInventoryMsg_Packet.h"

// Server to Client
RegisterZoneStruct("OP_ZoneInfoMsg", OP_ZoneInfoMsg_Packet, 283, 284, 942, 957, 959, 1067, 1096, 1142, 1193, 63119, 63182, 67650);
RegisterZoneStruct("OP_DoneSendingInitialEntitiesMsg", OP_DoneSendingInitialEntitiesMsg_Packet, 1);
RegisterZoneStruct("OP_ClientCmdMsg", OP_ClientCmdMsg_Packet, 283, 284);
RegisterZoneStruct("OP_EqSetPOVGhostCmd", OP_EqSetPOVGhostCmd_Packet, 1);
RegisterZoneStruct("OP_EqSetControlGhostCmd", OP_EqSetControlGhostCmd_Packet, 283, 284, 1096, 1119, 1193);
RegisterZoneStruct("OP_EqCreateGhostCmd", OP_CreateGhostCmd_Packet, 283, 284);
RegisterZoneStruct("OP_EqCreateWidgetCmd", OP_CreateWidgetCmd_Packet, 283, 284);
RegisterZoneStruct("OP_EqCreateSignWidgetCmd", OP_CreateSignWidgetCmd_Packet, 283, 284);
RegisterZoneStruct("OP_UpdateCharacterSheetMsg", OP_UpdateCharacterSheetMsg_Packet, 1);
RegisterZoneStruct("OP_DefaultGroupOptionsMsg", OP_DefaultGroupOptionsMsg_Packet, 1);
RegisterZoneStruct("OP_MoveableObjectPlacementCriteri", OP_MoveableObjectPlacementCriteri_Packet, 1);
RegisterZoneStruct("OP_GameWorldTimeMsg", OP_GameWorldTimeMsg_Packet, 1, 1193);
RegisterZoneStruct("OP_SetRemoteCmdsMsg", OP_SetRemoteCmdsMsg_Packet, 1);
RegisterZoneStruct("OP_EqUpdateGhostCmd", OP_UpdateGhostCmdMsg_Packet, 1);
RegisterZoneStruct("OP_TeleportWithinZoneNoReloadMsg", OP_TeleportWithinZoneNoReloadMsg_Packet, 1, 869);
RegisterZoneStruct("OP_ChangeZoneMsg", OP_ChangeZoneMsg_Packet, 1);
RegisterZoneStruct("OP_EqDestroyGhostCmd", OP_EqDestroyGhostCmd_Packet, 1);
RegisterZoneStruct("OP_ChangeServerControlFlagMsg", OP_ChangeServerControlFlagMsg_Packet, 1);
RegisterZoneStruct("OP_EqDisplayTextCmd", OP_DisplayTextCmd_Packet, 1, 284);
RegisterZoneStruct("OP_EqHearChatCmd", OP_HearChatCmd_Packet, 1, 284, 57080, 67650);
RegisterZoneStruct("OP_OnscreenMsgMsg", OP_OnscreenMsgMsg_Packet, 1, 284);
RegisterZoneStruct("OP_EqDialogOpenCmd", OP_DialogOpenCmd_Packet, 1, 1096);
RegisterZoneStruct("OP_CampStartedMsg", OP_CampStartedMsg_Packet, 1, 284, 1153);
RegisterZoneStruct("OP_CampAbortedMsg", OP_CampAbortedMsg_Packet, 1);
RegisterZoneStruct("OP_UpdateTitleCmd", OP_UpdateTitleCmd_Packet, 1, 1153);
RegisterZoneStruct("OP_EqCannedEmoteCmd", OP_EqCannedEmoteCmd_Packet, 1, 57080);
RegisterZoneStruct("OP_DialogCloseMsg", OP_DialogCloseMsg_Packet, 1);
RegisterZoneStruct("OP_EqHearPlayFlavorCmd", OP_HearPlayFlavorCmd_Packet, 1);
RegisterZoneStruct("OP_MapFogDataInitMsg", OP_MapFogDataInitMsg_Packet, 1188);
RegisterZoneStruct("OP_EqExamineInfoCmd", OP_EqExamineInfoCmd_Packet, 1, 1188);
RegisterZoneStruct("OP_UpdateInventoryMsg", OP_UpdateInventoryMsg_Packet, 1, 893, 1193, 1208, 57048, 57107, 67650);

// Client to Server
RegisterZoneStruct("OP_LoginByNumRequestMsg", OP_LoginByNumRequestMsg_Packet, 1, 1212);
RegisterZoneStruct("OP_DoneLoadingZoneResourcesMsg", OP_DoneLoadingZoneResourcesMsg_Packet, 1);
RegisterZoneStruct("OP_DoneLoadingEntityResourcesMsg", OP_DoneLoadingEntityResourcesMsg_Packet, 1);
RegisterZoneStruct("OP_DefaultGroupOptionsRequestMsg", OP_DefaultGroupOptionsRequestMsg_Packet, 1);
RegisterZoneStruct("OP_UpdatePositionMsg", OP_UpdatePositionMsg, 1, 284);
RegisterZoneStruct("OP_RemoteCmdMsg", OP_RemoteCmdMsg_Packet, 1);
RegisterZoneStruct("OP_PaperdollImage", OP_PaperdollImage_Packet, 1, 60000);
RegisterZoneStruct("OP_UpdateTargetMsg", OP_UpdateTargetMsg_Packet, 1);
RegisterZoneStruct("OP_ReadyToZoneMsg", OP_ReadyToZoneMsg_Packet, 1);
RegisterZoneStruct("OP_ChatFiltersMsg", OP_ChatFiltersMsg_Packet, 1);
RegisterZoneStruct("OP_RequestCampMsg", OP_RequestCampMsg_Packet, 1, 284, 1153);
RegisterZoneStruct("OP_StoodMsg", OP_StoodMsg_Packet, 1);
RegisterZoneStruct("OP_DialogSelectMsg", OP_DialogSelectMsg_Packet, 1);
RegisterZoneStruct("OP_EntityVerbsVerbMsg", OP_EntityVerbsVerbMsg_Packet, 1);
RegisterZoneStruct("OP_MapFogDataUpdateMsg", OP_MapFogDataUpdateMsg_Packet, 1188);
RegisterZoneStruct("OP_MapRequest", OP_MapRequest_Packet, 1);

//Bidirectional
RegisterZoneStruct("OP_PredictionUpdateMsg", OP_PredictionUpdateMsg_Packet, 1);
RegisterZoneStruct("OP_BioUpdateMsg", OP_BioUpdateMsg_Packet, 1);