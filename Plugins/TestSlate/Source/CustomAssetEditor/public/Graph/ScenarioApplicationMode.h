#pragma once

#include "CoreMinimal.h"
#include "ScenarioEditor.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "EditTabsUtil.h"
#include "ScenarioGraphEditorSummoner.h"

class FScenarioEditor;

class FScenarioApplicationMode : public FApplicationMode
{
	DECLARE_DELEGATE_RetVal_OneParam(TSharedRef<SGraphEditor>, FOnCreateGraphEditorWidget, UEdGraph*);

public:
	FScenarioApplicationMode(TSharedPtr<FScenarioEditor> InScenarioEditor);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;


protected:
	TWeakPtr<FScenarioEditor> ScenarioEditor;

	//可生成选项卡集
	FWorkflowAllowedTabSet ScenarioEditorTabFactories;

	FOnCreateGraphEditorWidget CreateGraphEditorCallback;
};

