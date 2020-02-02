// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestSlateCommands.h"

#define LOCTEXT_NAMESPACE "FTestSlateModule"

void FTestSlateCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "TestSlate", "Bring up TestSlate window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
