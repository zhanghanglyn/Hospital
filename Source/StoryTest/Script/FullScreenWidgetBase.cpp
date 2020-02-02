// Fill out your copyright notice in the Description page of Project Settings.


#include "FullScreenWidgetBase.h"

UFullScreenWidgetBase::UFullScreenWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UFullScreenWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFullScreenWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}