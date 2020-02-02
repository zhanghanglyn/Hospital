#include "ScenarioApplicationMode.h"

FScenarioApplicationMode::FScenarioApplicationMode(TSharedPtr<FScenarioEditor> InScenarioEditor)
	: FApplicationMode(FScenarioEditor::ScenarioMode , FScenarioEditor::GetLocalizedMode)
{
	ScenarioEditor = InScenarioEditor;

	//ScenarioEditorTabFactories.RegisterFactory(MakeShareable(new FScenarioGraphEditorSummoner(InScenarioEditor, CreateGraphEditorCallback)));

	TabLayout = FTabManager::NewLayout("Standalone_BehaviorTree_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(InScenarioEditor->GetToolbarTabId(), ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				//分一个GraphEditor区域
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f) //设置宽度系数
					->AddTab(FScenarioEditorTabsUtil::GraphEditorID ,ETabState::OpenedTab)
				)
				//分一个Details区域
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6f)
					->AddTab( FScenarioEditorTabsUtil::GraphDetailsID , ETabState::OpenedTab )
				)
			)
			/*->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f)
					->AddTab(TEXT("STory"), ETabState::ClosedTab)
				)
			)*/
		);
}

void FScenarioApplicationMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
	check(ScenarioEditor.IsValid());

	TSharedPtr<FScenarioEditor> ScenarioEditorPtr = ScenarioEditor.Pin();
	
	//终于在此调用了TabSpawners了！
	//ScenarioEditorPtr->RegisterTabSpawners(InTabManager.ToSharedRef());
	ScenarioEditorPtr->RegisterToolbarTab(InTabManager.ToSharedRef());

	//ScenarioEditorPtr->PushTabFactories(ScenarioEditorTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FScenarioApplicationMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();

	check(ScenarioEditor.IsValid());
	TSharedPtr<FScenarioEditor> ScenarioEditorPtr = ScenarioEditor.Pin();

	//ScenarioEditorPtr->SaveEditedObjectState();
}

void FScenarioApplicationMode::PostActivateMode()
{
	check(ScenarioEditor.IsValid());
	TSharedPtr<FScenarioEditor> ScenarioEditorPtr = ScenarioEditor.Pin();

	//ScenarioEditorPtr->RestoreStoryGraph();

	FApplicationMode::PostActivateMode();
}