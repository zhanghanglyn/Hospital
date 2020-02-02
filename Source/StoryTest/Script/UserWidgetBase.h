// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "CanvasPanel.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/ScaleBox.h"
#include "Engine/Classes/Engine/Texture2D.h"
#include "UMG/Public/Blueprint/WidgetTree.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "Public/Misc/DateTime.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class STORYTEST_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UUserWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

//界面按钮等相关控件变量
private:
	UCanvasPanelSlot *m_PanelSlot;

public:
	//设置UI位置
	UFUNCTION(BlueprintCallable)
	virtual void SetPosition(FVector2D pos);

	//尝试使用模板判断是否有setPosition函数
	template<typename T , void(T::*)(FVector2D) = &T::SetPosition>
	bool checkBePosition(T*) { return true; };
	bool checkBePosition(...) { return false; };
	template<typename U>
	void SetPosition(FVector2D pos)
	{
		U *tempSlot = Cast<U>(this->Slot);
		if (checkBePosition<U>(tempSlot) == true)
			tempSlot->SetPosition(pos);
		else
			UE_LOG(LogTemp, Warning, TEXT("This UPanelSlot Type Has No Func SetPosition!"));
	}
	
	//默认使用UCanvasPanel来进行
	UFUNCTION(BlueprintCallable)
	virtual void SetZorder(int32 zorder);

	template<typename T_Z , void(T_Z::*)(int32) = &T_Z::SetZOrder>
	bool checkBeZorder(T_Z*) { return true; };
	bool checkBeZorder(...) { return false; };
	template<typename U_Z>
	void SetZorder(int32 zorder)
	{
		U_Z *tempSlot = Cast<U_Z>(this->Slot);
		if (checkBeZorder<U_Z>(tempSlot) == true)
			tempSlot->SetZOrder(zorder);
		else
			UE_LOG(LogTemp, Warning, TEXT("This UPanelSlot Type Has No Func SetZorder!"));
	}

	//设置SetAnchors
	UFUNCTION(BlueprintCallable)
	virtual void SetAnchors(FAnchors InAnchors);

	template<typename T_A, void(T_A::*)(FAnchors) = &T_A::SetAnchors>
	bool checkBeAnchors(T_A*) { return true; };
	bool checkBeAnchors(...) { return false; };
	template<typename U_A>
	void SetAnchors(FAnchors InAnchors)
	{
		U_A *tempSlot = Cast<U_A>(this->Slot);
		if (checkBeAnchors<U_A>(tempSlot) == true)
			tempSlot->SetAnchors(InAnchors);
		else
			UE_LOG(LogTemp, Warning, TEXT("This UPanelSlot Type Has No Func SetAnchors!"));
	}

//私有属性等
private:
	FString UID;
	void CreateUID();

public:
	FString GetUID();

};
