// Fill out your copyright notice in the Description page of Project Settings.


#include "FullScreenRoot.h"

UFullScreenRoot::UFullScreenRoot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UFullScreenRoot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFullScreenRoot::NativeConstruct()
{
	Super::NativeConstruct();
}