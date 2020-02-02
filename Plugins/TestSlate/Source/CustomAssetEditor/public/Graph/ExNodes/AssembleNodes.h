#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ScenarioNodeNormal.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Mgr/ActiveComponentMgr.h"
#include "AssembleNodes.generated.h"

UENUM(BlueprintType)
enum class AssembleType : uint8
{
	EM_Assemble         UMETA(DisplayName = "装配组件"),
	EM_DELETE			UMETA(DisplayName = "删除组件"),
};

UCLASS()
class UAssembleNodes : public UScenarioNodeNormal
{
	GENERATED_BODY()

public:

	UAssembleNodes();

	//~ Begin UEdGraphNode Interface
	//创建一个新节点的时候调用，会在里面生成该节点的Instance
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//virtual void AllocateDefaultPins() override;
	//~ End

	virtual void OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent) override;
public:
	//互动物品
	UPROPERTY(EditAnywhere, Category = "装配组件")
	TSoftObjectPtr <AActor> ActiveOjb;

	UPROPERTY(EditAnywhere, Category = "装配组件")
	TSubclassOf < UEventComponentBase> ActiveComponent;

	UPROPERTY()
	UEventComponentBase* SaveDataComponnet;

	UPROPERTY(EditAnywhere, Category = "装配类型")
	AssembleType M_AssembleType;

protected:
	FName CurComponentName;

public:
	static const FName ComponentPropertyName;

};