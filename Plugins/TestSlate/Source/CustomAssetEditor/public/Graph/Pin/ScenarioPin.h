#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SGraphPin.h"

/* 因为继承的SGraphPin是继承SBorder，所以Pin是一个标准的Slate */
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