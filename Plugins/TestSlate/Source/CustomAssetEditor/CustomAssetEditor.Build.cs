// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomAssetEditor : ModuleRules
{
	public CustomAssetEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                //"CustomAssetEditor/public",
                //"CustomAssetEditor/public/Graph"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                //"CustomAssetEditor/private",
                //"CustomAssetEditor/private/Graph"
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "EditorStyle",
                "Json",
                "JsonUtilities",
                //为了资产新添加的
                "AssetRegistry",
                "EditorWidgets",

                "EditorStyle",
                "EditorWidgets",
                "Kismet",

                // graph stuff
				"GraphEditor",
                "ContentBrowser",

                "BlueprintGraph",
                "MessageLog",
                "CoreUObject",

                				// e.g. FPlatformApplicationMisc::ClipboardCopy
				"ApplicationCore",
                "PropertyEditor",
				// ... add private dependencies that you statically link with here ...	
			}
			);

        PrivateIncludePathModuleNames.AddRange(
            new string[]
            {
                "Settings",
                "IntroTutorials",
                "AssetTools",
                "LevelEditor"
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                "AssetTools",
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
