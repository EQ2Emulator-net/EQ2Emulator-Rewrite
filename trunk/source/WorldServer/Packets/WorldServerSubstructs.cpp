#include "stdafx.h"

#include "../../common/Packets/XmlStructDumper.h"
#include "OP_CreateCharacterRequestMsg_Packet.h"
#include "OP_WorldListMsg_Packet.h"

#define RegisterLoginSubstruct(pt, ...) RegisterXmlSubstruct(pt, "LoginStructs.xml", __VA_ARGS__)

RegisterLoginSubstruct(Substruct_CharacterCustomization_Asset, 1);
RegisterLoginSubstruct(Substruct_CharacterCustomization, 1, 284, 57080, 67650, 67804);
RegisterLoginSubstruct(Substruct_WorldListEntry, 1, 284, 1209, 60100, 67969);