
#include "CustomAssetEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "IDetailsView.h"
#include "EditorStyle.h"
#include "Editor/PropertyEditor/Public/PropertyEditorDelegates.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Modules/ModuleInterface.h"
#include "MyCustomAsset.h"

const FName CustomAssetEditorAppIdentifier = FName(TEXT("CustomAssetEditorApp")); //自定义资产编辑器应用程序标识符

#define LOCTEXT_NAMESPACE "CustomAssetEditor"

const FName FCustomAssetEditor::ToolkitFName(TEXT("CustomAssetEditor"));
const FName FCustomAssetEditor::PropertiesTabId(TEXT("CustomAssetEditor_Properties"));

void FCustomAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	/*
	//The workspace menu category of this toolkit  继承自FAssetEditorToolkit中的BaseToolkit
	TSharedPtr<FWorkspaceItem> WorkspaceMenuCategory;
	*/
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_CustomAssetEditor", "Custom Asset Editor"));

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// Register the properties tab spawner within our tab manager
	// We provide the function with the identiefier for this tab and a shared pointer to the
	// SpawnPropertiesTab function within this editor class
	// Additionaly, we provide a name to be displayed, a category and the tab icon
	InTabManager->RegisterTabSpawner(PropertiesTabId,
		FOnSpawnTab::CreateSP(this, &FCustomAssetEditor::SpawnPropertiesTab)) //注册一个当产生TAB时触发的委托
		.SetDisplayName(LOCTEXT("PropertiesTab", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FCustomAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(PropertiesTabId);
}

TSharedRef<SDockTab> FCustomAssetEditor::SpawnPropertiesTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == PropertiesTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.Label(LOCTEXT("GenericDetailsTitle", "Details"))
		.TabColorScale(GetTabColorScale())
		[
			// Provide the details view as this tab its content
			DetailsView.ToSharedRef()
		];
}

FCustomAssetEditor::~FCustomAssetEditor()
{
	DetailsView.Reset();
	PropertiesTab.Reset();
}


FName FCustomAssetEditor::GetToolkitFName() const
{
	return ToolkitFName;
}

FText FCustomAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Custom Asset Editor");
}

FText FCustomAssetEditor::GetToolkitName() const
{
	return LOCTEXT("AppLabelToolkit", "Custom Asset Editor");
}

FText FCustomAssetEditor::GetToolkitToolTipText() const
{
	return LOCTEXT("ToolTip", "Custom Asset Editor");
}

FString FCustomAssetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "AnimationDatabase ").ToString();
}

FLinearColor FCustomAssetEditor::GetWorldCentricTabColorScale() const
{
	return FColor::Red;
}

UMyCustomAsset* FCustomAssetEditor::GetCustomAsset()
{
	return CustomAsset;
}

void FCustomAssetEditor::SetCustomAsset(UMyCustomAsset* InCustomAsset) 
{
	CustomAsset = InCustomAsset;
}

void FCustomAssetEditor::InitCustomAssetEditorEditor(const EToolkitMode::Type Mode,
	const TSharedPtr< IToolkitHost>& InitToolkitHost,
	UMyCustomAsset* InCustomAsset)
{

	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;

	SetCustomAsset(InCustomAsset);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	// Create the layout of our custom asset editor
	TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_CustomAssetEditor_Layout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(),ETabState::OpenedTab)
			)
			// Split the tab and pass the tab id to the tab spawner
			->Split(
				FTabManager::NewSplitter()
				->Split
				(
					FTabManager::NewStack()
					->AddTab(PropertiesTabId, ETabState::OpenedTab)
				)
			)
		);

	FAssetEditorToolkit::InitAssetEditor(
		Mode,
		InitToolkitHost,
		CustomAssetEditorAppIdentifier,
		StandaloneDefaultLayout,
		true,
		true,
		(UObject*)InCustomAsset
	);
	if(DetailsView.IsValid())
		DetailsView->SetObject((UObject*)InCustomAsset);
}

#undef LOCTEXT_NAMESPACE