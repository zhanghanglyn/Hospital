#include "ScenarioEditor.h"
#include "ActionNodes.h"
#include "AssembleNodes.h"
#include "FunctionNode.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Data/NodeDataShowUI.h"
#include "Runtime/Slate/Public/Framework/Commands/GenericCommands.h"
#include "ConditionConversionNodeBase.h"

const FName FScenarioEditor::ToolkitFName(TEXT("CustomStoryEditor"));
const FName FScenarioEditor::PropertiesTabId(TEXT("CustomAssetEditor_Story"));
const FName FScenarioEditor::DetailsFNameId(TEXT("ScenarioDetail_Story"));
const FName FScenarioEditor::ParamDetailsId(TEXT("ParamDetailsId"));

const FName FScenarioEditor::ScenarioMode(TEXT("Scenario"));

#define LOCTEXT_NAMESPACE "CustomStoryEditor"

FName FScenarioEditor::GetToolkitFName() const
{
	return ToolkitFName;
}

FText FScenarioEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Custom Story Editor");
}

FText FScenarioEditor::GetToolkitName() const
{
	return LOCTEXT("AppLabelToolkit", "Custom Story Editor");
}

FText FScenarioEditor::GetToolkitToolTipText() const
{
	return LOCTEXT("ToolTip", "Custom Story Editor");
}

FString FScenarioEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "AnimationDatabase ").ToString();
}

FLinearColor FScenarioEditor::GetWorldCentricTabColorScale() const
{
	return FColor::Red;
}

//设置CustomAsset
void FScenarioEditor::SetCustomAsset(UMyCustomAsset* InCustomAsset)
{
	CustomAsset = InCustomAsset;
}

void FScenarioEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	//DocumentManager->SetTabManager(InTabManager);  19.12.23 屏蔽
	//FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);

	/* 19.12.23 新添加 */
	TabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_FSMAssetEditor", "FSM Asset Editor"));
	//const TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);

	//在这里注册一个NodeGraph
	InTabManager->RegisterTabSpawner(PropertiesTabId,
		FOnSpawnTab::CreateSP(this, &FScenarioEditor::SpawnGraphEditorTab)) //注册一个当产生TAB时触发的委托
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	//尝试一下注册多个，注册一个Detail
	InTabManager->RegisterTabSpawner(DetailsFNameId,
		FOnSpawnTab::CreateSP(this, &FScenarioEditor::SpawnPropertiesTab)) //注册一个当产生TAB时触发的委托
		.SetDisplayName(LOCTEXT("PropertiesTab", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	//再注册一个参数用ID
	InTabManager->RegisterTabSpawner(ParamDetailsId,
		FOnSpawnTab::CreateSP(this, &FScenarioEditor::SpawnParamDetailTab)) //注册一个当产生TAB时触发的委托
		.SetDisplayName(LOCTEXT("PropertiesParamTab", "ParamDetails"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
	

	//再注册一个toolbar
	RegisterToolbarTab(InTabManager);

}

void FScenarioEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(PropertiesTabId);
	InTabManager->UnregisterTabSpawner(DetailsFNameId);
	InTabManager->UnregisterTabSpawner(ParamDetailsId);
}

/*void FScenarioEditor::InitScenarioEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UMyCustomAsset* InCustomAsset)
{
	SetCustomAsset(InCustomAsset);

	TSharedPtr<FScenarioEditor> ThisPtr(SharedThis(this));
	if (!DocumentManager.IsValid())
	{
		DocumentManager = MakeShareable(new FDocumentTracker());
		// Creates a new tab manager
		DocumentManager->Initialize(ThisPtr);

		TSharedRef<FDocumentTabFactory> GraphEditorFactory = MakeShareable(new FScenarioGraphEditorSummoner(ThisPtr,
			FScenarioGraphEditorSummoner::FOnCreateGraphEditorWidget::CreateSP(this, &FScenarioEditor::CreateGraphEditorWidget)
		));

		// Also store off a reference to the grapheditor factory so we can find all the tabs spawned by it later.
		GraphEditorTabFactoryPtr = GraphEditorFactory;
		DocumentManager->RegisterDocumentFactory(GraphEditorFactory);
	}


	// if we are already editing objects, dont try to recreate the editor from scratch but update the list of objects in edition
	// ex: BehaviorTree may want to reuse an editor already opened for its associated Blackboard asset.
	const TArray<UObject*>* EditedObjects = GetObjectsCurrentlyBeingEdited();
	if (EditedObjects == nullptr || EditedObjects->Num() == 0)
	{
		//const TSharedRef<FTabManager::FLayout> DummyLayout = FTabManager::NewLayout("NullLayout")->AddArea(FTabManager::NewPrimaryArea());
		TSharedRef<FTabManager::FLayout> DummyLayout = FTabManager::NewLayout("Standalone_CustomAssetEditor_Layout_v1")
			->AddArea(
				FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.1f)
					->SetHideTabWell(true)
					->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
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

		//我觉得，之前就是缺少了这一步！！
		InitAssetEditor(Mode, InitToolkitHost, TEXT("ScenarioEditorTreeEditorApp"), DummyLayout,true,true, InCustomAsset);

		CreateInternalWidgets();

		AddApplicationMode(ScenarioMode, MakeShareable(new FScenarioApplicationMode(SharedThis(this))));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Scenario: EditdObjects IS Aleady EXIST"));
	}

	SetCurrentMode(ScenarioMode);


}
*/

void FScenarioEditor::InitScenarioEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UMyCustomAsset* InCustomAsset)
{
	SetCustomAsset(InCustomAsset);

	GraphEditorView = CreateGraphEditorWidgetNoDocument();
	//创建Details
	CreateInternalWidgets();
	/* 创建Param Detail */
	CreateParamWidgets();

	//创建UI
	TSharedRef<FTabManager::FLayout> DummyLayout = FTabManager::NewLayout("Standalone_ScenarioEditor_Layout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			/* 这是个Toolbar 但是居然没显示默认的 */
			->Split(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			// Split the tab and pass the tab id to the tab spawner
			->Split(
				FTabManager::NewSplitter()
				->Split
				(
					FTabManager::NewStack()
					->SetHideTabWell(true)
					->AddTab(PropertiesTabId, ETabState::OpenedTab)
				)
				->Split(
					/*FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->SetHideTabWell(true)
					->AddTab(DetailsFNameId, ETabState::OpenedTab)*/
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.6f)
						->SetHideTabWell(false)
						->AddTab(DetailsFNameId, ETabState::OpenedTab)
					)
					->Split(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.4f)
						->SetHideTabWell(false)
						->AddTab(ParamDetailsId, ETabState::OpenedTab)
					)
				)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, TEXT("ScenarioEditorTreeEditorApp"), DummyLayout, true, true, InCustomAsset);
}

/* 新测试不使用FDocumentTracker */
TSharedPtr< SGraphEditor> FScenarioEditor::CreateGraphEditorWidgetNoDocument()
{

	//创建一个Graph
	UScenarioGraph* MyGraph = Cast<UScenarioGraph>(CustomAsset->StoryGraph);
	if (MyGraph == NULL)
	{
		CustomAsset->StoryGraph = FBlueprintEditorUtils::CreateNewGraph(CustomAsset, TEXT("MyScenario Tree"), UScenarioGraph::StaticClass(), UScenarioGraphSchema::StaticClass());
		MyGraph = Cast<UScenarioGraph>(CustomAsset->StoryGraph);
		MyGraph->bAllowDeletion = false;

		//初始化编辑器的Graph
		const UScenarioGraphSchema* Schema = Cast<UScenarioGraphSchema>(MyGraph->GetSchema());
		Schema->CreateDefaultNodesForGraph(*MyGraph);
	}

	//把自身赋予给Graph变量
	MyGraph->ScenarioEditor = this;

	/* 以下开始创建GraphEditor */

	// 自定义Graph的显示
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("ScenarioCornerText_FSM","Scenario!");
	AppearanceInfo.InstructionText = LOCTEXT("ScenarioCornerText_FSM", "Scenario~");

	//创建一个点击事件
	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FScenarioEditor::OnSelectedNodesChanged);

	//CommandList相关
	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
			FExecuteAction::CreateSP(this, &FScenarioEditor::OnRenameNode),
			FCanExecuteAction::CreateSP(this, &FScenarioEditor::CanRenameNodes)
		);

		/*GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateSP(this, &FScenarioEditor::SelectAllNodes),
			FCanExecuteAction::CreateSP(this, &FScenarioEditor::CanSelectAllNodes)
		);*/

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FScenarioEditor::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FScenarioEditor::CanDeleteNodes)
		);

		//创建备注，标签
		/*GraphEditorCommands->MapAction(FGenericCommands::Get().Create,
			FExecuteAction::CreateSP(this, &FFSMAssetEditor::OnCreateComment)
		);*/

	}

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(MyGraph)
		.GraphEvents(InEvents)
	;
}

/* 创建GraphEditor! */
/*TSharedRef<SGraphEditor> FScenarioEditor::CreateGraphEditorWidget(UEdGraph* InGraph)
{
	check(InGraph != NULL);
	
	//创建一个点击事件
	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FScenarioEditor::OnSelectedNodesChanged);

	// Make title bar
	TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.FillWidth(1.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ScenarioGraphLabel", "Scenario"))
			.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
		]
		];
	
	return SNew(SGraphEditor)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(InGraph)
		.GraphEvents(InEvents)
		;

}*/

/* 在此创建GraphTab */
TSharedRef<SDockTab> FScenarioEditor::SpawnGraphEditorTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == PropertiesTabId);
	return SNew(SDockTab)
		.Label(LOCTEXT("EditorGraphCanvas", "Viewport"))
		[
			GraphEditorView.ToSharedRef()
		];

}

TSharedRef<SDockTab> FScenarioEditor::SpawnPropertiesTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == DetailsFNameId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.Label(LOCTEXT("GenericDetailsTitle", "Details"))
		.TabColorScale(GetTabColorScale())
		[
			// Provide the details view as this tab its content
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> FScenarioEditor::SpawnParamDetailTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == ParamDetailsId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.Label(LOCTEXT("GenericParamDetailsTitle", "ParamDetails"))
		.TabColorScale(GetTabColorScale())
		[
			// Provide the details view as this tab its content
			ParamDetailsView.ToSharedRef()
		];
}

/* 加载或者重新创建一个Graph */
/*void FScenarioEditor::RestoreStoryGraph()
{
	UScenarioGraph* MyGraph = Cast<UScenarioGraph>(CustomAsset->StoryGraph);
	const bool bNewGraph = MyGraph == NULL;
	if (MyGraph == NULL)
	{
		CustomAsset->StoryGraph = FBlueprintEditorUtils::CreateNewGraph(CustomAsset, TEXT("MyScenario Tree"), UScenarioGraph::StaticClass(), UScenarioGraphSchema::StaticClass());
		MyGraph = Cast<UScenarioGraph>(CustomAsset->StoryGraph);
		
		//初始化编辑器的Graph
		const UScenarioGraphSchema* Schema = Cast<UScenarioGraphSchema>(MyGraph->GetSchema());
		Schema->CreateDefaultNodesForGraph(*MyGraph);

	}

	//把MyGraph 做成一个新的有效负载包装,使用OpenDocument创建了一个新的TAB！
	TSharedRef<FTabPayload_UObject> Payload = FTabPayload_UObject::Make(MyGraph);
	TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(Payload, bNewGraph ? FDocumentTracker::OpenNewDocument : FDocumentTracker::RestorePreviousDocument);
}*/

void FScenarioEditor::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

/* 在此创建Details等内部的Widget */
void FScenarioEditor::CreateInternalWidgets()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::ObjectsUseNameArea, false);
	//DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	DetailsViewArgs.NotifyHook = this;
	//创建DetailsView
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(CustomAsset);
	DetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FScenarioEditor::IsPropertyEditable));
	//设置改Node名Node显示之类的？
	DetailsView->OnFinishedChangingProperties().AddSP(this, &FScenarioEditor::OnFinishedChangingProperties);
}

void FScenarioEditor::CreateParamWidgets()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::ObjectsUseNameArea, false);
	DetailsViewArgs.NotifyHook = this;
	//创建DetailsView
	ParamDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	ParamDetailsView->SetObject(NULL);
	ParamDetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FScenarioEditor::IsPropertyEditable));
	//设置改Node名Node显示之类的？
	ParamDetailsView->OnFinishedChangingProperties().AddSP(this, &FScenarioEditor::OnFinishedChangingParams);
}

FText FScenarioEditor::GetLocalizedMode(FName InMode)
{
	static TMap< FName, FText > LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(ScenarioMode, LOCTEXT("ScenarioMode", "Scenario"));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;
}

UMyCustomAsset* FScenarioEditor::GetCustomAsset() const
{
	return CustomAsset;
}

//在选择ITEM改变时调用
void FScenarioEditor::OnSelectedNodesChanged(const TSet<class UObject *>& NewSelection)
{
	//在这里添加点击Item时对DetailsView的改变 , 目前只写对一个Node的操作
	int32 SelectionNodeNum = NewSelection.Num();

	if (DetailsView.IsValid() == true)
	{
		if (SelectionNodeNum == 1)
		{
			for (UObject* Object : NewSelection)
			{
				DetailsView->SetObject(Object);
				CurFocusNode = Cast<UEdGraphNode>(Object);
				//根据当前的Node数据，更新Params
				UpdateParamDetailWhenNodeFocus(CurFocusNode);
			}
		}
		else
		{
			DetailsView->SetObject(CustomAsset);
			ParamDetailsView->SetObject(NULL);
		}
		
	}	
}

bool FScenarioEditor::IsPropertyEditable() const
{
	//UE_LOG(LogTemp, Warning, TEXT(" IsPropertyEditable"));
	return true;
}

FGraphPanelSelectionSet FScenarioEditor::GetSelectedNodes() const
{
	return GraphEditorView->GetSelectedNodes();
}

UEdGraphNode* FScenarioEditor::GetFirstSelectNode() const
{
	auto SelectedNodes = GraphEditorView->GetSelectedNodes();
	if (SelectedNodes.Num() == 0)
	{
		return nullptr;
	}
	return Cast<UEdGraphNode>(SelectedNodes[FSetElementId::FromInteger(0)]);
}

/*  修改了Detail属性后在此调用，进行Node更改
	20.1.2尝试获取修改的Property，对应不同的Property有不同的操作
*/
void FScenarioEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	GraphEditorView->GetCurrentGraph()->Modify();

	UE_LOG(LogTemp, Warning, TEXT(" Now The DetailsView Update ! UpdateNode!"));
	if (CurFocusNode)
	{
		CurFocusNode->Modify();

		//如果是更新连接节点
		if (UConditionConversionNodeBase* TransNode = Cast<UConditionConversionNodeBase>(CurFocusNode))
		{
			TransNode->OnDetailUpdate(PropertyChangedEvent);
		}
		else
		{
			UScenarioNodeNormal* NodeNormal = Cast<UScenarioNodeNormal>(CurFocusNode);
			if (NodeNormal)
			{
				NodeNormal->OnDetailUpdate(PropertyChangedEvent);
				//也同步更新一下底下的数据
				UpdateParamDetailWhenNodeFocus(CurFocusNode);

			}
		}
		
	}
}

/* 
当修改了FocusNode时，更新一下ParamDetail,为Detail设置可修改参数
*/
void FScenarioEditor::UpdateParamDetailWhenNodeFocus( UEdGraphNode* FocusNode)
{
	UScenarioNodeNormal* Node = Cast<UScenarioNodeNormal>(FocusNode);
	if (Node != nullptr)
	{
		//如果是Action节点，则设置为Component的参数
		if (Node->NodeCategory == FScenarioNodeUtil::NodeCategoryAction)
		{
			UActionNodes* ActionNode = Cast<UActionNodes>(Node);
			//ParamDetailsView->SetObject(ActionNode->DataBase);
			ParamDetailsView->SetObject(ActionNode->SaveDataComponnet);
		}
		//如果是Asseble节点，则设置为Component的参数
		else if (Node->NodeCategory == FScenarioNodeUtil::NodeCategoryAssembly)
		{
			UAssembleNodes* AssembleNode = Cast<UAssembleNodes>(Node);
			//ParamDetailsView->SetObject(ActionNode->DataBase);
			ParamDetailsView->SetObject(AssembleNode->SaveDataComponnet);
		}
		//如果是函数节点，
		else if (Node->NodeCategory == FScenarioNodeUtil::NodeCategoryFunction)
		{
			UFunctionNode* FunctionNode = Cast<UFunctionNode>(Node);
			ParamDetailsView->SetObject(FunctionNode->SaveData);
		}
		else
		{
			ParamDetailsView->SetObject(NULL);
		}
	}
}

/* 修改了Param属性后在此调用 */
void FScenarioEditor::OnFinishedChangingParams(const FPropertyChangedEvent& PropertyChangedEvent)
{

}

/* 在Graph中注册的事件相关 */
void FScenarioEditor::OnRenameNode()
{
	//auto bVisible = GraphEditorView->IsNodeTitleVisible(GetFirstSelectedNode(), /*bRequestRename = */true);
}

bool FScenarioEditor::CanRenameNodes() const
{
	auto SelectedNodes = GetSelectedNodes();
	if (SelectedNodes.Num() > 1) {
		return false;
	}

	return true;
	//return GetFirstSelectedNode()->bCanRenameNode;
}

void FScenarioEditor::DeleteSelectedNodes()
{
	//记录一次操作，可以c+z进行恢复
	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	GraphEditorView->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet Selections = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator Iter(Selections); Iter; ++Iter)
	{
		UEdGraphNode* CurNode = Cast<UEdGraphNode>(*Iter);
		if (CurNode->CanUserDeleteNode())
		{
			FBlueprintEditorUtils::RemoveNode(nullptr, CurNode, true);
		}
	}
}

bool FScenarioEditor::CanDeleteNodes() const
{
	const FGraphPanelSelectionSet Selections = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator Iter(Selections); Iter; ++Iter)
	{
		UEdGraphNode* CurNode = Cast<UEdGraphNode>(*Iter);
		if (CurNode->CanUserDeleteNode())
		{
			return true;
		}
	}
	return false;
}

/************************************************************************/
/*                      显示双击后的详情的界面					  	    */
/************************************************************************/
TSharedPtr<SGraphEditor> FScenarioEditor::CreateInsideNodeWidget(UEdGraph* Graph)
{
	// 自定义Graph的显示
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("ScenarioCornerText", "NodeDetail");
	AppearanceInfo.InstructionText = LOCTEXT("ScenarioCornerText", "NodeDetail");

	//创建一个点击事件
	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FScenarioEditor::OnSelectedNodesChanged);

	//CommandList相关
	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
			FExecuteAction::CreateSP(this, &FScenarioEditor::OnRenameNode),
			FCanExecuteAction::CreateSP(this, &FScenarioEditor::CanRenameNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FScenarioEditor::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FScenarioEditor::CanDeleteNodes)
		);
		//创建备注，标签
		/*GraphEditorCommands->MapAction(FGenericCommands::Get().Create,
			FExecuteAction::CreateSP(this, &FFSMAssetEditor::OnCreateComment)
		);*/
	}
	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(Graph)
		.GraphEvents(InEvents)
		;
}

//测试2 再重新打开一个Tab再原始窗口上
void FScenarioEditor::OpenInsideNodeGraphTab(UScenarioNodeNormal* Node)
{
	//创建一个Graph
	if (Node->InsideGraph == NULL)
	{
		UEdGraph* CreateGraph = FBlueprintEditorUtils::CreateNewGraph(Node, TEXT("Inside Node"), UScenarioGraph::StaticClass(), UScenarioGraphSchema::StaticClass());
		Node->InsideGraph = Cast<UScenarioGraph>(CreateGraph);
		Node->InsideGraph->bAllowDeletion = true;

		//初始化编辑器的Graph
		//const UScenarioGraphSchema* Schema = Cast<UScenarioGraphSchema>(NodeGraph->GetSchema());
		//Schema->CreateDefaultNodesForGraph(*MyGraph);
	}

	//判断下，如果已经打开过，则不需要打开，focus到Tab上即可
	int TabNum = -1;

	for (int i = 0; i < DialogTabs.Num(); i++)
	{
		SGraphEditor& rGraphEditor = (SGraphEditor&)DialogTabs[i]->GetContent().Get();
		if (rGraphEditor.GetCurrentGraph() == Node->InsideGraph)
		{
			TabNum = i;
			break;
		}
	}
	//创建一个Tab
	TSharedPtr<SDockTab> InsideNodeTab;

	//之后应该要根据节点类型来创建自定义的Graph
	if (TabNum == -1)
	{
		TSharedPtr<SGraphEditor> DialogEditor = CreateInsideNodeWidget(Node->InsideGraph);
		//创建一个Tab
		InsideNodeTab = SNew(SDockTab)
			.Label(FText::FromString( "Inside : " + Node->NodeName))
			.TabRole(ETabRole::DocumentTab)
			.TabColorScale(GetTabColorScale())
			[
				DialogEditor.ToSharedRef()
			];

		DialogTabs.Add(InsideNodeTab);
	}
	else
	{
		InsideNodeTab = DialogTabs[TabNum];
		InsideNodeTab->RequestCloseTab();
	}
	
	TabManager->InsertNewDocumentTab(PropertiesTabId, FTabManager::ESearchPreference::Type::PreferLiveTab, InsideNodeTab.ToSharedRef());
	TabManager->DrawAttention(InsideNodeTab.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE