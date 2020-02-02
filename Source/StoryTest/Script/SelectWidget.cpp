// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"


USelectWidget::USelectWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

//�ڴ�˳������²�ͬ�÷�
void USelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Ϊ��ť1���¼�
	if (Select_1 != NULL)
	{
		Select_1->OnClicked.__Internal_AddDynamic(this, &USelectWidget::SelectOneClick, FName("SelectOneClick"));
	}

	if (Select_2 != NULL)
	{
		Select_2->OnClicked.__Internal_AddDynamic(this, &USelectWidget::SelectTwoClick, FName("SelectOneClick"));
	}

	//Select_3 = Cast<UButton>(GetWidgetFromName(FName(TEXT("Select_3"))));
	if (Select_3 != NULL)
	{
		Select_3->OnClicked.__Internal_AddDynamic(this, &USelectWidget::SelectThreeClick, FName("SelectThreeClick"));
	}

}

void USelectWidget::SetCurPageData(TArray<FString> text_list, TArray<int32> page_list)
{
	m_TextList = TArray<FString>(text_list);
	m_PageList = TArray<int32>(page_list);
}


/************************************************************************/
/*				������ť����¼����                                    */
/************************************************************************/
void USelectWidget::SelectOneClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Select 1 Has Been touched!!!!!"));

	SelectParam *temp_param = new SelectParam();
	temp_param->pageId = m_PageList[0];
	temp_param->SelectId = m_TextList[0];

	m_clickOneDelegate.ExecuteIfBound(*temp_param);
}

void USelectWidget::SelectTwoClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Select 2 Has Been Touched!!!!"));

	SelectParam *temp_param = new SelectParam();
	temp_param->pageId = m_PageList[1];
	temp_param->SelectId = m_TextList[1];

	m_clickOneDelegate.ExecuteIfBound(*temp_param);
}

void USelectWidget::SelectThreeClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Select 3 Has Been Touched!!!!"));

	SelectParam *temp_param = new SelectParam();
	temp_param->pageId = m_PageList[2];
	temp_param->SelectId = m_TextList[2];

	m_clickOneDelegate.ExecuteIfBound(*temp_param);
}

/************************************************************************/
/*                     �󶨵���¼�										*/
/************************************************************************/
void USelectWidget::UnBindClick()
{
	m_clickOneDelegate.Unbind();
}