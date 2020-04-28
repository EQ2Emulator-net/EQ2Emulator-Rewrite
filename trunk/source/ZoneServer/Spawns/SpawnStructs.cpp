#include "stdafx.h"

#include "SpawnStructs.h"
#include "Spawn.h"

void Substruct_SpawnVisualization::InsertSpawnData(const std::shared_ptr<Spawn>& spawn) {
	const SpawnVisualizationStruct* vis = spawn->GetVisStruct();
	static_cast<SpawnVisualizationStruct&>(*this) = *vis;

	vis_flags = 54;
	//if (vis->bHideHood)
}