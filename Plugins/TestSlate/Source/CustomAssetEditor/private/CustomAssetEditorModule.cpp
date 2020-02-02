#include "CustomAssetEditorModule.h"
#include "Toolkits/IToolkit.h"
#include "ModuleManager.h"
#include "CustomAssetEditor.h"
#include "ScenarioEditor.h"

#define LOCTEXT_NAMESPACE "CustomAssetEditorModule"

//const FName CustomAssetEditorAppIdentifier = FName(TEXT("CustomAssetEditorApp")); //自定义资产编辑器应用程序标识符

class FCustomAssetEditorModule : public ICustomAssetEditorModule
{
public:
	FCustomAssetEditorModule() {};

	/* 继承子IModuleInterface的接口 */
	virtual void StartupModule() override {
		MenuExtensibliltyManager = MakeShareable(new FExtensibilityManager);
		ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

		//先注册一个自定义分类
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		FAssetTypeActions_MyCustomAsset::AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName("EventTree"), LOCTEXT("EventAssetCategory", "EventTree"));

		RegisterAssetTypeAction( MakeShareable(new FAssetTypeActions_MyCustomAsset()));

		GraphPinFactory = MakeShared<FScenarioPinFactory>();
		FEdGraphUtilities::RegisterVisualPinFactory(GraphPinFactory);
		GraphNodeFactory = MakeShared<FScenarioNodeFactory>();
		FEdGraphUtilities::RegisterVisualNodeFactory(GraphNodeFactory);
		GraphConnectFactory = MakeShared<FScenarioConnectionFactory>();
		FEdGraphUtilities::RegisterVisualPinConnectionFactory(GraphConnectFactory);

	};

	virtual void ShutdownModule() override {

		FEdGraphUtilities::UnregisterVisualPinFactory(GraphPinFactory);
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphNodeFactory);
		FEdGraphUtilities::UnregisterVisualPinConnectionFactory(GraphConnectFactory);

		MenuExtensibliltyManager.Reset();
		MenuExtensibliltyManager.Reset();

		UnregisterAssetTypeAction();

	}

	virtual TSharedPtr< FExtensibilityManager> GetMenuExtensibilityManager() override {
		return MenuExtensibliltyManager;
	};

	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override {
		return ToolBarExtensibilityManager;
	};

	/* 创建一个AssetEditor */
	virtual  TSharedRef<ICustomAssetEditor> CreateCustomAssetEditor(
		const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost>& InitToolkitHost,
		UMyCustomAsset* CustomAsset) override
	{
		// Initialize and spawn a new custom asset editor with the provided parameters
		TSharedRef< FCustomAssetEditor > NewCustomAssetEditor(new FCustomAssetEditor());
		NewCustomAssetEditor->InitCustomAssetEditorEditor(Mode, InitToolkitHost, CustomAsset);
		return NewCustomAssetEditor;
	}
	
	/* 创建一个自定义的剧情播放器 */
	virtual TSharedRef<FScenarioEditor> CreateCustomStoryEditor(
		const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost>& InitToolkitHost,
		UMyCustomAsset* CustomAsset) override;

#pragma optimize("",off)
	//模块被写进去的时候就要吧Asset注册进去！
	void RegisterAssetTypeAction(TSharedRef<IAssetTypeActions> Actions);
	
#pragma optimize("",on)

	void UnregisterAssetTypeAction()
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (int32 listCount = 0; listCount < ActionList.Num(); listCount)
			{
				AssetTools.UnregisterAssetTypeActions(ActionList[listCount].ToSharedRef());
			}
		}
		ActionList.Empty();
	}


protected:
	TSharedPtr< FExtensibilityManager> MenuExtensibliltyManager;
	TSharedPtr< FExtensibilityManager> ToolBarExtensibilityManager;

	//记录所有被注册的AssetTypeActions
	TArray< TSharedPtr<IAssetTypeActions>> ActionList;
};

IMPLEMENT_GAME_MODULE(FCustomAssetEditorModule, CustomAssetEditor)

void FCustomAssetEditorModule::RegisterAssetTypeAction(TSharedRef<IAssetTypeActions> Actions)
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetTools.RegisterAssetTypeActions(Actions);
	ActionList.Add(Actions);
}

#pragma optimize("",off)
TSharedRef<FScenarioEditor> FCustomAssetEditorModule::CreateCustomStoryEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UMyCustomAsset* CustomAsset)
{
	TSharedRef< FScenarioEditor > NewCustomAssetEditor(new FScenarioEditor());
	NewCustomAssetEditor->InitScenarioEditor(Mode, InitToolkitHost, CustomAsset);
	return NewCustomAssetEditor;
}
#pragma optimize("",on)

#undef LOCTEXT_NAMESPACE