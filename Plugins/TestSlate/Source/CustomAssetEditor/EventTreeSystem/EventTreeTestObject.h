#pragma once
#include "CoreMinimal.h"
#include "EventTreeMgr.h"
#include "UObject/ObjectMacros.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "EventTreeTestObject.generated.h"

//事件系统测试函数
UCLASS()
class AEventTreeTestObject : public AActor
{
	GENERATED_BODY()

public:
	AEventTreeTestObject()
	{
		//TreeMgr = NewObject<UEventTreeMgr>();

	};

	UFUNCTION(BlueprintCallable)
	void StartAAA()
	{
		if (TreeMgr == nullptr)
			TreeMgr = NewObject<UEventTreeMgr>();
		//if (TreeMgr->GetTreeAsset(AssetPath))
			//TreeMgr->StartRun();
	};

	UPROPERTY()
	UEventTreeMgr* TreeMgr;

	UPROPERTY(EditAnywhere, Category = "EventTrees")
	FString AssetPath;
};