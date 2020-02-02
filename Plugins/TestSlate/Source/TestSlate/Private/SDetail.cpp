
#include "SDetail.h"

#define  LOCTEXT_NAMESPACE "SDetail"

void SDetail::Construct(const FArguments& InArgs)
{
	//绑定一下委托
	ClickDelegate = InArgs._Clicked;

	this->ChildSlot
	[
		SNew(SVerticalBoxEx).ContentMargin(FMargin(20,50))
		.BGImage(FTestSlateStyle::Get().GetBrush("UI.VerticalBoxExBg")).Clipping(EWidgetClipping::ClipToBounds)
		+SVerticalBoxEx::Slot()
		.MaxHeight(20)
		[
			//SNew(SImage).Image(FTestSlateStyle::Get().GetBrush("UI.IMAGEtest1") )
			SNew(STextBlock).Text(FText::FromString(TEXT("测试属性2")))
		]
		+ SVerticalBoxEx::Slot()
		.MaxHeight(40)
		[
			SAssignNew(InputPtr ,SEditableTextBox).HintText(FText::FromString(TEXT("输入测试")))
			.Style( &FTestSlateStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("UI.EditBoxStyle") )
		]
		+SVerticalBoxEx::Slot()
		.MaxHeight(40)
		[
			SNew(SButton).Text(FText::FromString(TEXT("点击保存"))).OnClicked(this, &SDetail::ArrowCreateTest )
		]
		//SAssignNew(LayoutPtr, SVerticalBoxEx)

	];
}

FReply SDetail::ClickSave()
{
	if (InputPtr != nullptr)
		InputPtr->GetText().ToString();

	return FReply::Handled();
}

FReply SDetail::ArrowCreateTest()
{
	ClickDelegate.ExecuteIfBound();


	return FReply::Handled();
}

#undef   LOCTEXT_NAMESPACE