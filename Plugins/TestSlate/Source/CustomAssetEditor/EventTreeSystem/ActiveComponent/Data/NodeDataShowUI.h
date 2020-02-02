#pragma once
#include "CoreMinimal.h"
#include "ComponentNodeDataBase.h"
#include "NodeDataShowUI.generated.h"

/*
在Node中进行数据存储的类，每一个Component应该对应一个数据类
该数据类与节点共存进行数据存储,所有属性均标记为可在Detail中设置
*/
UCLASS(BlueprintType)  //BlueprintType
class UNodeDataShowUI : public UComponentNodeDataBase
{
	GENERATED_BODY()

public:
	UNodeDataShowUI() {
		DataCatogary = FComponentDataCategoryUtil::DataCategoryShowUI;
	};


public:
	UPROPERTY(EditAnywhere, Category = "互动组件属性")
	FString ShowName;

	UPROPERTY(EditAnywhere, Category = "互动组件属性")
	FString JustForFun;

	//一定有一个对应的类型
	//UPROPERTY()
	//FName DataCatogary = FComponentDataCategoryUtil::DataCategoryBase;
};