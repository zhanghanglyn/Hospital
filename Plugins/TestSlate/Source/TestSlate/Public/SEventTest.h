/**
@ 类描述
@Author zhangh
@Email  1
@Date   2019-10-24 11:29:09

1
*/


#pragma once
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "TestSlate/Public/TestSlateStyle.h"

DECLARE_DELEGATE_TwoParams(FClickLoginDelegate , FString , FString )

class  SEventTest : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEventTest){}
	SLATE_EVENT(FClickLoginDelegate, OnClickTest)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnclickLogin();

private:
	FClickLoginDelegate onClickLoginDelegate;
	TSharedPtr<SEditableTextBox> m_TextUserName;
	TSharedPtr<SEditableTextBox> m_TextPassWord;
	TSharedPtr<SButton> m_LoginBtn;
};