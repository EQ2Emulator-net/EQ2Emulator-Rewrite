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

#define RegisterZoneSubstruct(pt, ...) RegisterXmlSubstruct(pt, "ZoneStructs.xml", __VA_ARGS__)

//Order these so struct dependencies are registered before their dependents
RegisterZoneSubstruct(Substruct_MovementData, 283, 284);
RegisterZoneSubstruct(Substruct_SpawnHeader, 283, 284, 57080);
RegisterZoneSubstruct(Substruct_SpawnPosition, 284, 284, 911);
RegisterZoneSubstruct(Substruct_TargetSpellInfo, 1, 60055);
RegisterZoneSubstruct(Substruct_SpawnInfo, 283, 284, 860, 861, 864, 936, 955, 1096, 1188, 1198, 60055);
RegisterZoneSubstruct(Substruct_SpawnVisualization, 1, 936, 955, 1142, 1188, 60055);
RegisterZoneSubstruct(Substruct_Lerp, 1);
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
//I would rather these 2 substructs not be inlined but analyzer has issues nesting arrays that deep
//RegisterZoneSubstruct(Substruct_MapFogData, 60114);
//RegisterZoneSubstruct(Substruct_MapData, 60114);