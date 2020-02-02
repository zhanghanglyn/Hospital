// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ScenarioScenarioConfig.generated.h"

/**
 * 
 */
USTRUCT()
struct STORYTEST_API FScenarioScenarioConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString scenarioId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString se_path;
};

USTRUCT(BlueprintType)
struct FStorySelectConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString EventId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString SelectText;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString Choose;

};