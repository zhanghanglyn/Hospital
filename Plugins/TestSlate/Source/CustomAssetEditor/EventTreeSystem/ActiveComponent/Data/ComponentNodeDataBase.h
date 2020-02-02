#pragma once
#include "CoreMinimal.h"
#include "EditTabsUtil.h"
#include "ComponentNodeDataBase.generated.h"

/*
在Node中进行数据存储的类，每一个Component应该对应一个数据类
该数据类与节点共存进行数据存储,所有属性均标记为可在Detail中设置
*/
UCLASS(BlueprintType)
class UComponentNodeDataBase : public UObject
{
	GENERATED_BODY()

public:
	UComponentNodeDataBase() {};


public:
	//一定有一个对应的类型
	UPROPERTY()
	FName DataCatogary = FComponentDataCategoryUtil::DataCategoryBase;

	//UPROPERTY(EditAnywhere, Category = "父类测试用属性")
	//FString TTTTTTT;
};