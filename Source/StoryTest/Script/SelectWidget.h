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

	//��ʼ����ǰ����¼�����
	void SetCurPageData(TArray<FString> text_list, TArray<int32> page_list);

//�������
public:
	UFUNCTION()
	void SelectOneClick();
	
	UFUNCTION()
	void SelectTwoClick();

	UFUNCTION()
	void SelectThreeClick();

//�󶨰�ť
public:
	UPROPERTY(VisibleAnyWhere,BlueprintReadWrite ,meta = (BindWidget))
	UButton *Select_1;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Select_2;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Select_3;   //ͨ������ȡ

//�����ť�Ļص�ί��
public:

	DECLARE_DELEGATE_OneParam(ClickOneDelegate, SelectParam);
	ClickOneDelegate m_clickOneDelegate;

	UFUNCTION()
	void UnBindClick();


protected:

	TArray<FString> m_TextList;
	TArray<int32> m_PageList;
};
