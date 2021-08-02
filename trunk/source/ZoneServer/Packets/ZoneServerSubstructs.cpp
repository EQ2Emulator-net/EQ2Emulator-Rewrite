#include "stdafx.h"

#include "../../common/Packets/XmlStructDumper.h"
#include "OP_UpdatePositionMsg.h"
#include "Substruct_SpawnHeader.h"
#include "../Spawns/SpawnStructs.h"
#include "OP_UpdateCharacterSheetMsg_Packet.h"
#include "Substruct_GroupMember.h"
#include "OP_UpdateSpawnCmdMsg.h"
#include "OP_MapFogDataUpdateMsg_Packet.h"
#include "OP_MapFogDataInitMsg_Packet.h"
#include "ItemPackets.h"
#include "OP_PlayFlythroughCmd_Packet.h"
#include "Substruct_SpellDesc.h"
#include "Substruct_AADesc.h"
#include "Substruct_ExamineDescSpell.h"
#include "OP_EqUpdateMerchantCmd_Packet.h"
#define RegisterZoneSubstruct(pt, ...) RegisterXmlSubstruct(pt, "ZoneStructs.xml", __VA_ARGS__)

//Order these so struct dependencies are registered before their dependents
RegisterZoneSubstruct(Substruct_MovementData, 283, 284, 1144);
RegisterZoneSubstruct(Substruct_SpawnHeader, 283, 284, 57080);
RegisterZoneSubstruct(Substruct_SpawnPosition, 283, 284, 843, 911, 1096);
RegisterZoneSubstruct(Substruct_TargetSpellInfo, 1, 864, 865, 60055);
RegisterZoneSubstruct(Substruct_SpawnInfo, 283, 284, 843, 860, 861, 864, 936, 955, 1008, 1096, 1188, 1198, 60055, 67633, 67804);
RegisterZoneSubstruct(Substruct_SpawnVisualization, 1, 843, 936, 955, 1008, 1142, 1188, 60055);
RegisterZoneSubstruct(Substruct_Lerp, 1);
RegisterZoneSubstruct(Substruct_Matrix3, 1);
RegisterZoneSubstruct(Substruct_Position, 1);
RegisterZoneSubstruct(Substruct_SpawnTitleStruct, 1, 865);
RegisterZoneSubstruct(Substruct_SpawnFooter, 1, 865);
RegisterZoneSubstruct(Substruct_WidgetFooter, 1);
RegisterZoneSubstruct(Substruct_SignFooter, 283, 284, 60070);
RegisterZoneSubstruct(Substruct_SpellEffects, 1, 843, 1193);
RegisterZoneSubstruct(Substruct_PassiveEffects, 1, 1188);
RegisterZoneSubstruct(Substruct_MaintainedEffects, 1, 1193, 57048);
RegisterZoneSubstruct(Substruct_GroupMember, 843, 844, 1199, 57048, 61532, 69195);
RegisterZoneSubstruct(Substruct_GroupMemberAppearance, 1199, 60114);
RegisterZoneSubstruct(Substruct_GroupMemberMapLocation, 1199, 60114);
RegisterZoneSubstruct(Substruct_GroupSheet, 1, 1188, 67650);
RegisterZoneSubstruct(Substruct_RaidSheet, 1);
RegisterZoneSubstruct(Substruct_SpellProps, 843, 844, 1199, 60114, 67650, 67804);
RegisterZoneSubstruct(Substruct_TsSpellProps, 1199, 60114);
RegisterZoneSubstruct(UpdateCharacterSheetMsgData, 1, 843, 865, 1208, 60114, 67650, 67730, 67804, 69195);
RegisterZoneSubstruct(Substruct_SpawnInfoUpdate, 1);
RegisterZoneSubstruct(Substruct_SpawnPosUpdate, 1, 1188);
RegisterZoneSubstruct(Substruct_SpawnVisUpdate, 1);
RegisterZoneSubstruct(Substruct_ItemDescHeader, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ItemDescFooter, 1191, 60114, 67650, 67727);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Generic, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_AchievementProfile, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Adornment, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Ammo, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Armor, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Bag, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Bauble, 1191, 60114, 67650, 67727);
RegisterZoneSubstruct(Substruct_HouseItem, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_House, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Book, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_HouseContainer, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_MeleeWeapon, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Provision, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_ReforgingDecoration, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RangedWeapon, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RecipeBook, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RewardCrate, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RewardVoucher, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Shield, 1191, 60114, 67650);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_PlaySound, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_Text, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_ActivateCamera, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_DeactivateCamera, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_SetCamera, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_SceneFade, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_LensFilterState, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Action_LensFilter, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Shot, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Movement_Strings, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Movement, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc_Scene, 1);
RegisterZoneSubstruct(Substruct_FlythroughDesc, 1);
RegisterZoneSubstruct(Substruct_SpellDesc, 1208, 60114, 67650, 69195);
RegisterZoneSubstruct(Substruct_ExamineDescItem_SpellScroll, 1208, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescSpell, 1208, 60114, 67650);
RegisterZoneSubstruct(Substruct_AADesc, 1193);
RegisterZoneSubstruct(Substruct_MapFogData, 1188);
RegisterZoneSubstruct(Substruct_MapData, 1188);
RegisterZoneSubstruct(Substruct_MerchantItem, 67650);