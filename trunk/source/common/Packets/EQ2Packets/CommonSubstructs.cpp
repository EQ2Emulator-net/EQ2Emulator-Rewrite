#include "stdafx.h"

#include "../XmlStructDumper.h"
#include "Substruct_NetAppearance.h"

#define RegisterCommonSubstruct(pt, ...) RegisterXmlSubstruct(pt, "CommonStructs.xml", __VA_ARGS__)

RegisterCommonSubstruct(Substruct_NetAppearance, 1, 284, 1193, 60114, 67650);