// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TestSlateStyle.h"

class FTestSlateCommands : public TCommands<FTestSlateCommands>
{
public:

	FTestSlateCommands()
		: TCommands<FTestSlateCommands>(TEXT("TestSlate"), NSLOCTEXT("Contexts", "TestSlate", "TestSlate Plugin"), NAME_None, FTestSlateStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};