/**
@ 类描述
@Author zhangh
@Email  1
@Date   2019-10-31 12:02:08

1
*/


#pragma once
#include "TestSlate.h"

class  SLayoutDetail : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLayoutDetail){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:


public:

	void CreateArrowTest();


};