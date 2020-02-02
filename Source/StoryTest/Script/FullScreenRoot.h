// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Script/FullScreenWidgetBase.h"
#include "FullScreenRoot.generated.h"

/**
 * 
 */
UCLASS()
class UFullScreenRoot : public UFullScreenWidgetBase
{
	GENERATED_BODY()
	
public:
	UFullScreenRoot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel *Root;   //一定会有一个根节点！
};
