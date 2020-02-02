// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StoryTest : ModuleRules
{
	public StoryTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",
            "InputCore", "HeadMountedDisplay" , "LevelSequence", "MovieScene" ,
            "UMG",
            "RTSMode",
            "CustomAssetEditor" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" , "RenderCore", "RHI", "CustomAssetEditor" });
    }
}
