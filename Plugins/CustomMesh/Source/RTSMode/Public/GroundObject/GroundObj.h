#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"
#include "GroundObj.generated.h"

/*
	包含格子信息的地面
	暂时先默认一个底面对应一个GridMgr
*/
UCLASS(BlueprintType, Blueprintable)
class RTSMODE_API AGroundObj : public AActorBase
{
	GENERATED_BODY()
public:
	AGroundObj( FString InActorName );

	/** Constructor for AActor that takes an ObjectInitializer for backward compatibility */
	AGroundObj(const FObjectInitializer& ObjectInitializer);

	/* virtual */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	/* 开始被点击 */
	virtual void StartTouch(FVector TouchLocation) override;
	virtual void TouchHold(FVector TouchLocation) override;
	virtual void TouchEnd(FVector TouchLocation) override;
public:

	//设置右下角的起点位置（用来计算相对位置）,会根据当前尺寸等自动计算 
	FVector GetRelativeStart();

protected:
	UPROPERTY(VisibleAnywhere , meta = (Category = "Ground Mesh"))
	class UStaticMeshComponent* GroundMeshComponent;
	//模型路径 暂时这么标
	UPROPERTY(EditAnywhere, meta = (Category = "Ground Mesh"))
	FString GroundMeshPath = "/Game/StarterContent/Shapes/Shape_Plane";

	UPROPERTY(EditAnywhere, meta = (Category = "Ground Config"))
	FVector2D GroundWidthHeight = FVector2D(100,100);

	UPROPERTY()
	class UGroundGridMgrComponent* GridMgr;
};
