#include "stdafx.h"

#include "../XmlStructDumper.h"
#include "Substruct_NetAppearance.h"

#define RegisterCommonSubstruct(pt, ...) RegisterXmlSubstruct(pt, "CommonStructs.xml", __VA_ARGS__)

RegisterCommonSubstruct(Substruct_NetAppearance, 283, 284, 844, 1193, 60114, 67650, 67804);