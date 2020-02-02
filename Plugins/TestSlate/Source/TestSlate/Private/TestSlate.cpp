// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestSlate.h"
#include "TestSlateStyle.h"
#include "TestSlateCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName TestSlateTabName("TestSlate");

#define LOCTEXT_NAMESPACE "FTestSlateModule"

void FTestSlateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTestSlateStyle::Initialize();
	FTestSlateStyle::ReloadTextures();

	FTestSlateCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTestSlateCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FTestSlateModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FTestSlateModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());
		ToolbarExtender->AddToolBarExtension("Content", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FTestSlateModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestSlateTabName, FOnSpawnTab::CreateRaw(this, &FTestSlateModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FTestSlateTabTitle", "TestSlate"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FTestSlateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FTestSlateStyle::Shutdown();

	FTestSlateCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TestSlateTabName);
}

TSharedRef<SDockTab> FTestSlateModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FTestSlateModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("TestSlate.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			/*SNew(SHorizontalBox)
			//添加左侧Detail
			+ SHorizontalBox::Slot()
			.MaxWidth(200)
			.HAlign(HAlign_Left)
			[
				SNew(SLayoutDetail)
			]
			//添加右侧部分
			+ SHorizontalBox::Slot()
			.MaxWidth(700)
			.HAlign(HAlign_Left)
			[
				SNew(SWidgetTestA)
			]*/

			SAssignNew(m_SCanvas, SCanvas)
			+SCanvas::Slot().Position(FVector2D( 50,50 )).Size(FVector2D(200,400))
			[
				SNew(SLayoutDetail)
			]
			+SCanvas::Slot().Position( FVector2D( 300,100)).Size(FVector2D( 400,400 ))
			[
				SNew(SCanvasTree, FVector2D(300, 100) , FVector2D(400,400))
				//+SCanvasTree::Slot().Position(FVector2D(10, 10))
				//[
					//SNew(STreeNode)
				//]
				/*+ SCanvasTree::Slot().Position(FVector2D(100, 100))
				[
					SNew(STreeNode)
				]
				+ SCanvasTree::Slot().Position(FVector2D(50, 50))
				[
					SNew(STreeNode)
				]*/
			]

		];
}

void FTestSlateModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(TestSlateTabName);
}

void FTestSlateModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FTestSlateCommands::Get().OpenPluginWindow);
}

void FTestSlateModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FTestSlateCommands::Get().OpenPluginWindow);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestSlateModule, TestSlate)