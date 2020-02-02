// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "CanvasPanel.h"
#include "StoryTest/DataUtil/EventEnum.h"
#include "SelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class STORYTEST_API USelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	USelectWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	//初始化当前点击事件数据
	void SetCurPageData(TArray<FString> text_list, TArray<int32> page_list);

//点击方法
public:
	UFUNCTION()
	void SelectOneClick();
	
	UFUNCTION()
	void SelectTwoClick();

	UFUNCTION()
	void SelectThreeClick();

//绑定按钮
public:
	UPROPERTY(VisibleAnyWhere,BlueprintReadWrite ,meta = (BindWidget))
	UButton *Select_1;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Select_2;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Select_3;   //通过名字取

//点击按钮的回调委托
public:

	DECLARE_DELEGATE_OneParam(ClickOneDelegate, SelectParam);
	ClickOneDelegate m_clickOneDelegate;

	UFUNCTION()
	void UnBindClick();


protected:

	TArray<FString> m_TextList;
	TArray<int32> m_PageList;
};
