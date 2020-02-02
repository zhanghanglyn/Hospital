#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "ICustomAssetEditor.h"

class IDetailsView;
class SDockableTab;
class UMyCustomAsset;

//CUSTOMASSETEDITOR_API 
class FCustomAssetEditor : public ICustomAssetEditor
{

	// This function creates tab spawners on editor initialization
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	// This function unregisters tab spawners on editor initialization
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
public:
	/*
		编辑制定资产对象
		Param : Mode			    此编辑器的资产编辑模式(独立或以世界为中心)
		@param	InitToolkitHost     当模式以世界为中心时，这是生成并包含该编辑器的LevelEditor实例
		@param	InCustomAsset		要编辑的自定义资产
	*/
	void InitCustomAssetEditorEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost>& InitToolkitHost,
		UMyCustomAsset* CustomAsset);

	virtual UMyCustomAsset* GetCustomAsset();

	virtual void SetCustomAsset(UMyCustomAsset* InCustomAsset);

	/* 一下方法继承自FAssetEditorToolkit */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual bool IsPrimaryEditor() const override { return true; }
	/* OVER */

	virtual ~FCustomAssetEditor();

private:
	/* 在TabManaget中注册的，产生Tab的委托函数 */
	TSharedRef<SDockTab> SpawnPropertiesTab(const FSpawnTabArgs& Args);
	
	TSharedPtr<SDockableTab> PropertiesTab;

	TSharedPtr<class IDetailsView> DetailsView;

	/**	The tab ids for all the tabs used */
	static const FName PropertiesTabId;

	static const FName ToolkitFName;

	/** The Custom Asset open within this editor */
	UMyCustomAsset* CustomAsset;
};