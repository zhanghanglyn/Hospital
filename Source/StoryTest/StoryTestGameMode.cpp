// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StoryTestGameMode.h"
#include "StoryTestHUD.h"
#include "StoryTestCharacter.h"
#include "StoryTest/Script/StoryPlayerPawn.h"
#include "RTSMode/Private/RTSControl/RTSPlayerCameraSpectatorPawn.h"
#include "RTSMode/Private/RTSControl/RTSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AStoryTestGameMode::AStoryTestGameMode()
	: Super()
{

	PlayerControllerClass = ARTSPlayerController::StaticClass();
	DefaultPawnClass = ARTSPlayerCameraSpectatorPawn::StaticClass();

	// use our custom HUD class
	HUDClass = AStoryTestHUD::StaticClass();
}
