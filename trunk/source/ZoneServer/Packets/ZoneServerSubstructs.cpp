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

#define RegisterZoneSubstruct(pt, ...) RegisterXmlSubstruct(pt, "ZoneStructs.xml", __VA_ARGS__)

//Order these so struct dependencies are registered before their dependents
RegisterZoneSubstruct(Substruct_MovementData, 283, 284);
RegisterZoneSubstruct(Substruct_SpawnHeader, 283, 284, 57080);
RegisterZoneSubstruct(Substruct_SpawnPosition, 284, 284, 911);
RegisterZoneSubstruct(Substruct_TargetSpellInfo, 1, 60055);
RegisterZoneSubstruct(Substruct_SpawnInfo, 283, 284, 860, 861, 864, 936, 955, 1096, 1188, 1198, 60055);
RegisterZoneSubstruct(Substruct_SpawnVisualization, 1, 936, 955, 1142, 1188, 60055);
RegisterZoneSubstruct(Substruct_Lerp, 1);
RegisterZoneSubstruct(Substruct_Matrix3, 1);
RegisterZoneSubstruct(Substruct_Position, 1);
RegisterZoneSubstruct(Substruct_SpawnTitleStruct, 1);
RegisterZoneSubstruct(Substruct_SpawnFooter, 1);
RegisterZoneSubstruct(Substruct_WidgetFooter, 1);
RegisterZoneSubstruct(Substruct_SignFooter, 283, 284, 60070);
RegisterZoneSubstruct(Substruct_SpellEffects, 1, 843, 1193);
RegisterZoneSubstruct(Substruct_PassiveEffects, 1, 1193);
RegisterZoneSubstruct(Substruct_MaintainedEffects, 1, 1193, 57048);
RegisterZoneSubstruct(Substruct_GroupMember, 1, 1188, 57048, 61532);
RegisterZoneSubstruct(Substruct_GroupSheet, 1);
RegisterZoneSubstruct(UpdateCharacterSheetMsgData, 1, 60114);
RegisterZoneSubstruct(Substruct_SpawnInfoUpdate, 1);
RegisterZoneSubstruct(Substruct_SpawnPosUpdate, 1, 1188);
RegisterZoneSubstruct(Substruct_SpawnVisUpdate, 1);
RegisterZoneSubstruct(Substruct_ItemDescHeader, 60114, 67650);
RegisterZoneSubstruct(Substruct_ItemDescFooter, 60114, 67650, 67727);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Generic, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_AchievementProfile, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Adornment, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Ammo, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Armor, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Bag, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Bauble, 60114, 67650, 67727);
RegisterZoneSubstruct(Substruct_HouseItem, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_House, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Book, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_HouseContainer, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_MeleeWeapon, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Provision, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_ReforgingDecoration, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RangedWeapon, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RecipeBook, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RewardCrate, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_RewardVoucher, 60114, 67650);
RegisterZoneSubstruct(Substruct_ExamineDescItem_Shield, 60114, 67650);
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
//I would rather these 2 substructs not be inlined but analyzer has issues nesting arrays that deep
//RegisterZoneSubstruct(Substruct_MapFogData, 1188);
//RegisterZoneSubstruct(Substruct_MapData, 1188);