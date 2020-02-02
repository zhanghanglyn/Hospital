#include "GroundGridMgrComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGroundGridMgrComponent::UGroundGridMgrComponent()
{

}

UGroundGridMgrComponent::UGroundGridMgrComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UGroundGridMgrComponent::InitGridData(FVector2D InWidthHeight)
{
	GridWidthNum = UKismetMathLibrary::FFloor(InWidthHeight.X / FGroundUtil::GroundGridWidth);	//一行有多少格子
	GridHeightNum = UKismetMathLibrary::FFloor(InWidthHeight.Y / FGroundUtil::GroundGridHeight);	

	WidthHeight = FVector2D(InWidthHeight);

	//初始化GridList
	for (int32 Row = 0; Row < GridWidthNum; ++Row)
	{
		for (int32 Column = 0; Column < GridHeightNum; ++Column)
		{
			FGridData NewGridData;
			NewGridData.SetGridId();

			GridDataList.Add(NewGridData);
			if (GridDataListNested.Num() <= Row)
			{
				FNestedArray tempArray;
				GridDataListNested.Add(tempArray);
			}
			GridDataListNested[Row].Array.Add( NewGridData);
		}
	}
}

#pragma optimize("",off)
void UGroundGridMgrComponent::GetTouchGrid(FVector TouchLocation , FGridData& GridData)
{
	//判断相对位置落在哪个格子区间 ,
	int32 CurGridColumn = UKismetMathLibrary::FCeil(TouchLocation.X / FGroundUtil::GroundGridWidth);
	int32 CurGridRow = UKismetMathLibrary::FCeil(TouchLocation.Y / FGroundUtil::GroundGridHeight);

	int32 GridIndex = (CurGridRow - 1) * GridWidthNum + CurGridColumn - 1;  //从0开始，所以-1
	if (GridDataList.Num() >= GridIndex)
	{
		GridData = GridDataList[GridIndex];

		GridData = GridDataListNested[CurGridRow - 1].Array[CurGridColumn - 1];
		UE_LOG(LogTemp, Warning , TEXT("aaaaaa"));
	}

}
#pragma optimize("",on)

