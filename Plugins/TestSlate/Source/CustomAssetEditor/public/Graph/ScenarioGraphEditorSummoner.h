#pragma once 

#include "CoreMinimal.h"
#include "ScenarioEditor.h"
#include "EdGraph/EdGraph.h"
#include "GraphEditor.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

/* UEdGraph就是整个的图，所以其实很多可以理解为激活这个图的时候的函数 */
struct FScenarioGraphEditorSummoner : FDocumentTabFactoryForObjects<UEdGraph>
{
public:
	DECLARE_DELEGATE_RetVal_OneParam(TSharedRef<SGraphEditor>, FOnCreateGraphEditorWidget, UEdGraph*);

public:
	FScenarioGraphEditorSummoner(TSharedPtr<class FScenarioEditor> ScenarioEditorPtr, FOnCreateGraphEditorWidget CreateGraphEditorWidgetCallback);
	/*继承自父类的父类FDocumentTabFactory的方法，会在Tab被这个工厂创建的时候激活*/
	virtual void OnTabActivated(TSharedPtr<SDockTab> Tab) const override;
	/*当从该工厂创建的选项卡应该刷新时调用*/
	virtual void OnTabRefreshed(TSharedPtr<SDockTab> Tab) const override;

protected:
	virtual TAttribute<FText> ConstructTabNameForObject(UEdGraph* DocumentID) const override;
	virtual TSharedRef<SWidget> CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const override;
	virtual const FSlateBrush* GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const override;
	virtual void SaveState(TSharedPtr<SDockTab> Tab, TSharedPtr<FTabPayload> Payload) const override;

protected:
	TWeakPtr<class FScenarioEditor> ScenarioEditorPtr;
	FOnCreateGraphEditorWidget OnCreateGraphEditorWidget;
};