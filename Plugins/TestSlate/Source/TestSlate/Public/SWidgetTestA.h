/**
@ 测试控件slate
@Author zhangh
@Email  1
@Date   2019-10-23 17:27:59

1
*/

#pragma once
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "SEventTest.h"
#include "Widgets/SBoxPanel.h"
#include "SMyButton.h"

class  SWidgetTestA : public SCompoundWidget
{
private:

	//TAttribute<FString> TestAttribute1;

public:
	SLATE_BEGIN_ARGS(SWidgetTestA){}
	SLATE_ATTRIBUTE(FString, TestAttribute1)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	//测试点击函数
	FReply OnclickButton1();

	void OnClickLogin(FString name, FString psw);
};