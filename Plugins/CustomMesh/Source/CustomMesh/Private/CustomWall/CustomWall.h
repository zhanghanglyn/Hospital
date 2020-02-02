#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
//#include "Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "CustomWall.generated.h"

UCLASS(BlueprintType, Blueprintable , ClassGroup = CustomMesh)
class CUSTOMMESH_API ACustomWall : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	//ACustomWall(const FObjectInitializer& ObjectInitializer);

	/* virtual */
	virtual void PostActorCreated() override;

	
public:
	void SetStartPoint( FVector InStartVec);
	void SetEndPoint(FVector InStartVec);

	//对外方法，根据传入的长和宽，绘制立方体
	void CreateWall(FVector BoxRadius);
protected:

protected:
	UProceduralMeshComponent* WallMesh;

private:
	//起始点和结束点
	FVector StartVec;
	FVector EndVec;
};