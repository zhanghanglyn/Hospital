
#include "SEventTest.h"

#define  LOCTEXT_NAMESPACE "SEventTest"

void SEventTest::Construct(const FArguments& InArgs)
{
	onClickLoginDelegate = InArgs._OnClickTest;

	this->ChildSlot.Padding(50,50,50,50)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			SNew(SImage).Image(
				FTestSlateStyle::Get().GetBrush("UI.IMAGEtest1")
			)
		]
		+ SVerticalBox::Slot().Padding(0, 0, 60, 60).AutoHeight()
		[
			SAssignNew(m_TextUserName, SEditableTextBox)
			.Style(
				&FTestSlateStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("UI.EditBoxStyle")
			).HintText(FText::FromString(TEXT("请输入用户名")))
		]
		+SVerticalBox::Slot()
		.Padding(0,0,60,60).AutoHeight()
		[
			SAssignNew(m_TextPassWord, SEditableTextBox)
			.Style(
				&FTestSlateStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>( "UI.EditBoxStyle" )
			).HintText(FText::FromString(TEXT("请输入密码"))).IsPassword(true)
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(m_LoginBtn, SButton).OnClicked(this, &SEventTest::OnclickLogin).Text(FText::FromString(TEXT("登陆")))
			.ButtonStyle(		//设置BUTTON样式
				&FTestSlateStyle::Get().GetWidgetStyle<FButtonStyle>( "UI.Button1Style")
			)
		]

	];
}

FReply SEventTest::OnclickLogin()
{
	//UE_LOG(LogTemp, Warning, TEXT("user Name is : %s , PassWord is : %s") , m_TextUserName->GetText(), m_TextPassWord->GetText());
	onClickLoginDelegate.ExecuteIfBound(m_TextUserName->GetText().ToString(), m_TextPassWord->GetText().ToString());
	return FReply::Handled();
}

#undef   LOCTEXT_NAMESPACE