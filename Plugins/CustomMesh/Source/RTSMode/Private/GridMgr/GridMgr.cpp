#include "GridMgr.h"

int32 FGridData::GridCount = 0;
FString FGridData::GridName = "Grid_";

bool FGridData::SetGridId()
{
	if (GridId.IsEmpty())
	{
		GridCount++;
		GridId = GridName + FString::FromInt(GridCount);
		return true;
	}

	return false;
}