#pragma once

#include "CoreMinimal.h"
#include "Kismet/Public/WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "Runtime/CoreUObject/Public/Misc/NotifyHook.h"
#include "MyCustomAsset.h"
#include "ScenarioGraphEditorSummoner.h"
#include "ScenarioGraph.h"
#include "SWidget.h"
#include "EdGraph/EdGraphNode.h"
#include "ScenarioGraphSchema.h"
#include "Editor/EditorStyle/Public/EditorStyle.h"
#include "Slate/Public/Widgets/Docking/SDockTab.h"
#include "Kismet/Public/BlueprintEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "ScenarioApplicationMode.h"
//#include "ScenarioNodeNormal.h"
#include "Editor/PropertyEditor/Public/IDetailsView.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Kismet/Public/WorkflowOrientedApp/WorkflowTabManager.h"

class FScenarioEditor : public FWorkflowCentricApplication , public FEditorUndoClient ,public FNotifyHook
{
public:

	/*
		编辑制定资产对象
		Param : Mode			    此编辑器的资产编辑模式(独立或以世界为中心)
		@param	InitToolkitHost     当模式以世界为中心时，这是生成并包含该编辑器的LevelEditor实例
		@param	InCustomAsset		要编辑的自定义资产
	*/
	void InitScenarioEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost>& InitToolkitHost,
		UMyCustomAsset* InCustomAsset);

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	/* 一下方法继承自FAssetEditorToolkit */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual bool IsPrimaryEditor() const override { return true; }
	/* OVER */

	// @todo This is a hack for now until we reconcile the default toolbar with application modes [duplicated from counterpart in Blueprint Editor]
	void RegisterToolbarTab(const TSharedRef<class FTabManager>& TabManager);

	/* 创建时调用 */
	void OnGraphEditorFocused(const TSharedRef<SGraphEditor>& InGraphEditor) {  };
	/** Restores the Story graph we were editing or creates a new one if none is available */
	//void RestoreStoryGraph();

	/**
	 * Get the localized text to display for the specified mode
	 * @param	InMode	The mode to display
	 * @return the localized text representation of the mode
	 */
	static FText GetLocalizedMode(FName InMode);

	UMyCustomAsset* GetCustomAsset() const;
	void SetCustomAsset(UMyCustomAsset* InCustomAsset);

	/* 在TabManaget中注册的，产生SGraphEditor的委托函数 */
	TSharedRef<SDockTab> SpawnGraphEditorTab(const FSpawnTabArgs& Args);

	/* 在TabManaget中注册的，产生Tab的委托函数 */
	TSharedRef<SDockTab> SpawnPropertiesTab(const FSpawnTabArgs& Args);

	/* 创建一个详情参数Tab */
	TSharedRef<SDockTab> SpawnParamDetailTab(const FSpawnTabArgs& Args);

	void OpenInsideNodeGraphTab(UScenarioNodeNormal* Node);

protected:
	//初始化完InsideNode后，初始化一个InsideView
	TSharedPtr<class SGraphEditor> CreateInsideNodeWidget(UEdGraph* Graph);

private:
	//在选择ITEM改变时调用
	virtual void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

private:
	/** Create widget for graph editing */
	//TSharedRef<class SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);

	/* 新测试不使用FDocumentTracker */
	TSharedPtr< SGraphEditor> CreateGraphEditorWidgetNoDocument();

	/*创建内部Widget，Details*/
	void CreateInternalWidgets();
	/* DetailsView更新相关 */
	bool IsPropertyEditable() const;
	/*当发生属性变化时的更新*/
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

	/* 创建Param detail */
	void CreateParamWidgets();
	/*当发生属性变化时的更新*/
	void OnFinishedChangingParams(const FPropertyChangedEvent& PropertyChangedEvent);
	/* 当重新选择节点的时候，根据节点类型以及数据更新ParamDetails */
	void UpdateParamDetailWhenNodeFocus( class UEdGraphNode* FocusNode );

	/*获取当前选中的Nodes*/
	FGraphPanelSelectionSet GetSelectedNodes() const;
	UEdGraphNode* GetFirstSelectNode() const;

	/* 在Graph中注册的事件相关 */
	void OnRenameNode();
	bool CanRenameNodes() const;
	void DeleteSelectedNodes();
	bool CanDeleteNodes() const;

public:
	/* 当前正在编辑的资源，MyCustomAsset */
	UMyCustomAsset* CustomAsset;
	//剧情模块ID
	static const FName ScenarioMode;

	/* 19.12.23 新测试使用，创建的GraphEditor */
	TSharedPtr< SGraphEditor> GraphEditorView;

	/* 用来显示双击打开的详情的Graph */
	TSharedPtr< SGraphEditor> NodeInsideView;

private:
	/**	The tab ids for all the tabs used */
	static const FName PropertiesTabId;

	static const FName ToolkitFName;
	/* 创建一个Details的ID */
	static const FName DetailsFNameId;
	/* 创建参数Details */
	static const FName ParamDetailsId;

	TSharedPtr<FDocumentTracker> DocumentManager;
	TWeakPtr<FDocumentTabFactory> GraphEditorTabFactoryPtr;

	/* 属性Details */
	TSharedPtr<class IDetailsView> DetailsView;
	/* 属性详细参数Details */
	TSharedPtr<class IDetailsView> ParamDetailsView;

	/* 当前Focus的Node */
	class UEdGraphNode* CurFocusNode;

	/** Command list for the graph editor */
	TSharedPtr<class FUICommandList> GraphEditorCommands;

	//存储一个额外打开的SDockTabList
	TArray<TSharedPtr<SDockTab>> DialogTabs;
};