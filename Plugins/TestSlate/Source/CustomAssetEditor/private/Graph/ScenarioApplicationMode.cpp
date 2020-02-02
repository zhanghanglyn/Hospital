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
				//��һ��GraphEditor����
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f) //���ÿ��ϵ��
					->AddTab(FScenarioEditorTabsUtil::GraphEditorID ,ETabState::OpenedTab)
				)
				//��һ��Details����
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
	
	//�����ڴ˵�����TabSpawners�ˣ�
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