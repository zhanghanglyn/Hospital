#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SGraphPin.h"

/* ��Ϊ�̳е�SGraphPin�Ǽ̳�SBorder������Pin��һ����׼��Slate */
class SScenarioPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SScenarioPin) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

public:
	/** Build the widget we should put into the 'default value' space, shown when nothing connected */
	virtual TSharedRef<SWidget>	GetDefaultValueWidget();

};