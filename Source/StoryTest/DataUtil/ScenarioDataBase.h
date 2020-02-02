// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ScenarioDataBase.generated.h"

/**
 * 
 */
USTRUCT()
struct FScenarioDataBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FScenarioDataBase();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		int32 mId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		int32 nextPage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		int32 second_temp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString caption;
	//抛出事件ID
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString eventId;		
	//是否不控制镜头
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		bool bOutCameraControl;
	//选项列表
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString select;
	//选项跳转页面
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString selectToPage;

	//事件参数
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")

};
