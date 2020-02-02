#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ScenarioNodeNormal.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Data/ComponentNodeDataBase.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Mgr/ActiveComponentMgr.h"
#include "ActionNodes.generated.h"

UCLASS()
class UActionNodes : public UScenarioNodeNormal
{
	GENERATED_BODY()

public:

	UActionNodes();

	//~ Begin UEdGraphNode Interface
	//创建一个新节点的时候调用，会在里面生成该节点的Instance
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End

	virtual void OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent) override;
public:
	//互动物品
	UPROPERTY(EditAnywhere, Category = "互动组件")
	TSoftObjectPtr <AActor> ActiveOjb;

	UPROPERTY(EditAnywhere, Category = "互动组件")
	TSubclassOf < UEventComponentBase> ActiveComponent;

	UPROPERTY()
	UEventComponentBase* SaveDataComponnet;

	//因为所有的TSubclassOf都是一样的，想当于是一个模板类型，所以！！！！试下用单独的数据来存放
	//UPROPERTY()
	//UComponentNodeDataBase* DataBase;

protected:
	FName CurComponentName;

public:
	static const FName ComponentPropertyName;

};