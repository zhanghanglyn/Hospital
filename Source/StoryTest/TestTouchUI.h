// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Script/UserWidgetBase.h"
#include "Engine/Classes/Engine/AssetManager.h"
#include "Engine/Classes/Materials/Material.h"
#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TestTouchUI.generated.h"

/**
 * 
 */
UCLASS()
class STORYTEST_API UTestTouchUI : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UTestTouchUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

//控件变量
public:
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel *RootPanel;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel *BF_Root;

	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel *RootAnima;

	UPROPERTY(BlueprintReadWrite)
	UImage *BF_Image;

	UPROPERTY(BlueprintReadWrite)
	UImage *BF_Anima;

//控件相关
public:

	UFUNCTION()
	void GetAllCtl();

	//更换设置的图片并且开启一段简单动画
	UFUNCTION(BlueprintCallable)
	void SetPlay();

//变量
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString fs = "I AM TestTouchUI!!!";


};
