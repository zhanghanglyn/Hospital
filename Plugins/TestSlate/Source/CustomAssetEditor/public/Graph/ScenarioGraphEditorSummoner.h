#pragma once 

#include "CoreMinimal.h"
#include "ScenarioEditor.h"
#include "EdGraph/EdGraph.h"
#include "GraphEditor.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

/* UEdGraph����������ͼ��������ʵ�ܶ�������Ϊ�������ͼ��ʱ��ĺ��� */
struct FScenarioGraphEditorSummoner : FDocumentTabFactoryForObjects<UEdGraph>
{
public:
	DECLARE_DELEGATE_RetVal_OneParam(TSharedRef<SGraphEditor>, FOnCreateGraphEditorWidget, UEdGraph*);

public:
	FScenarioGraphEditorSummoner(TSharedPtr<class FScenarioEditor> ScenarioEditorPtr, FOnCreateGraphEditorWidget CreateGraphEditorWidgetCallback);
	/*�̳��Ը���ĸ���FDocumentTabFactory�ķ���������Tab���������������ʱ�򼤻�*/
	virtual void OnTabActivated(TSharedPtr<SDockTab> Tab) const override;
	/*���Ӹù���������ѡ�Ӧ��ˢ��ʱ����*/
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