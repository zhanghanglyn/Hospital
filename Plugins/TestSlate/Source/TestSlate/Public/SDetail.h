/**
@ 类描述
@Author zhangh
@Email  1
@Date   2019-10-31 14:31:35

1
*/


#pragma once
#include "TestSlate.h"
#include "TestSlate/Private/SVerticalBoxEx/SVerticalBoxEx.h"
#include "SAutoLayout.h"

//点击按钮后向上传递一个委托
DECLARE_DELEGATE(CreateArrowDelegate)

class  SDetail : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDetail){}
	SLATE_EVENT(CreateArrowDelegate , Clicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:

	TSharedPtr<SVerticalBoxEx> LayoutPtr;
	TSharedPtr<SEditableTextBox> InputPtr;
	TSharedPtr<SButton> BtnPtr;

	//点击委托
	CreateArrowDelegate ClickDelegate;

public:
	FReply ClickSave();
	//测试生成箭头点击的函数
	FReply ArrowCreateTest();


};