// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Script/FullScreenWidgetBase.h"
#include "FullScreenTest.generated.h"

/**
 * 
 */
UCLASS()
class STORYTEST_API UFullScreenTest : public UFullScreenWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void TestFunc();
};
