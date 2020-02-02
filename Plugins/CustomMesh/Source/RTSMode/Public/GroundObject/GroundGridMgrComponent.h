#pragma once

#include "CoreMinimal.h"
#include "GroundUtil.h"
#include "Components/ActorComponent.h"
#include "GridMgr.h"
#include "NestedArray.h"
#include "GroundGridMgrComponent.generated.h"

/* 
	地板格子数据的管理类
	地板格子的起始点，就是地板Mesh的00点，与mesh的大小一一对应（但是生成时是使用的FLOOR()）
*/
UCLASS()
class UGroundGridMgrComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGroundGridMgrComponent();
	UGroundGridMgrComponent(const FObjectInitializer& ObjectInitializer);


	void InitGridData( FVector2D InWidthHeight);
	//设置当前格子的左上角起始点在世界空间中的位置（和地面一致）
	void InitGridStartLocation(FVector InStartLocation) { StartLocation = InStartLocation; };

	//根据传入的点击位置获取点击到的格子
	void GetTouchGrid( FVector TouchLocation, FGridData& GridData);

protected:

	UPROPERTY()
	int32 GridWidthNum = 0;	//横向格子数量

	UPROPERTY()
	int32 GridHeightNum = 0;//纵向格子数量

	UPROPERTY()
	FVector2D WidthHeight = FVector2D::ZeroVector;

	UPROPERTY()
	FVector StartLocation;	//当前格子管理的左上角起始点在世界空间中的位置（和地面一致）

	UPROPERTY()
	TArray<FGridData> GridDataList;	//格子list

	//测试用NestArray
	UPROPERTY()
	TArray< FNestedArray > GridDataListNested;
};