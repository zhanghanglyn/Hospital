#include "ScenarioGraphSchema.h"
#include "ScenarioGraph.h"
#include "UObject/MetaData.h"
#include "UObject/UnrealType.h"
#include "UObject/TextProperty.h"
#include "Editor/GraphEditor/Public/GraphEditorActions.h"
#include "Runtime/Slate/Public/Framework/Commands/GenericCommands.h"
#include "RootNodes.h"
#include "EditTabsUtil.h"
#include "End_SchemaAction.h"
#include "Action_SchemaAction.h"
#include "Assemble_SchemaAction.h"
#include "Function_SchemaAction.h"
#include "ConditionConversionNodeBase.h"
#if WITH_EDITOR
#include "Misc/ConfigCacheIni.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectIterator.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ScopedTransaction.h"
#include "EditorCategoryUtils.h"
#endif

#define LOCTEXT_NAMESPACE "UScenarioGraphSchema"

//定义两个Pin的类型
//const FName UScenarioGraphSchema::PC_Normal(TEXT("Normal"));
//const FName UScenarioGraphSchema::PC_Root(TEXT("Root"));

UScenarioGraphSchema::UScenarioGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UScenarioGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	//FGraphNodeCreator<UScenarioNodeBase> NodeCreater(Graph);
	//UScenarioNodeBase* Nodebase = NodeCreater.CreateNode();
	//NodeCreater.Finalize();
	FGraphNodeCreator<URootNodes> NodeCreater(Graph);
	URootNodes* Nodebase = NodeCreater.CreateNode();
	NodeCreater.Finalize();

	//设置RootNode
	UScenarioGraph* ScenarioGraph = Cast<UScenarioGraph>(&Graph);
	if (ScenarioGraph)
		ScenarioGraph->SetRootNode(Nodebase);
}

void UScenarioGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FEndSchemaAction> EndAction = TSharedPtr<FEndSchemaAction>(
		new FEndSchemaAction(LOCTEXT("EventCategory", "StandNode"), LOCTEXT("EndNodes", "End Nodes"), FText::GetEmpty(), 1, nullptr, FScenarioNodeUtil::NodeCategoryEnd)
		);

	ContextMenuBuilder.AddAction(EndAction);

	/*TSharedPtr<FScenarioSchemaAction> NewSchemaAction = TSharedPtr<FScenarioSchemaAction>(
		new FScenarioSchemaAction(LOCTEXT("CustomStoryCategory", "EventNode"), LOCTEXT("Nodes", "Normal Nodes"), FText::GetEmpty(), 0, nullptr, FScenarioNodeUtil::NodeCategoryNormal)
		);

	ContextMenuBuilder.AddAction(NewSchemaAction);*/

	//添加互动节点
	TSharedPtr<FActionSchemaAction> ActionSchemaAction = TSharedPtr<FActionSchemaAction>(
		new FActionSchemaAction(LOCTEXT("CustomStoryCategory", "事件节点"), LOCTEXT("ActionNodes", "行为组件节点"), FText::GetEmpty(), 0, nullptr, FScenarioNodeUtil::NodeCategoryAction)
		);
	TSharedPtr<FAssembleSchemaAction> AssembleSchemaAction = TSharedPtr<FAssembleSchemaAction>(
		new FAssembleSchemaAction(LOCTEXT("CustomStoryCategory", "事件节点"), LOCTEXT("AssembleNodes", "装配组件节点"), FText::GetEmpty(), 0, nullptr, FScenarioNodeUtil::NodeCategoryAssembly)
		);
	TSharedPtr<FFunctionSchemaAction> FunctionSchemaAction = TSharedPtr<FFunctionSchemaAction>(
		new FFunctionSchemaAction(LOCTEXT("CustomStoryCategory", "事件节点"), LOCTEXT("FunctionNodes", "函数组件节点"), FText::GetEmpty(), 0, nullptr, FScenarioNodeUtil::NodeCategoryFunction)
		);

	ContextMenuBuilder.AddAction(ActionSchemaAction);
	ContextMenuBuilder.AddAction(AssembleSchemaAction);
	ContextMenuBuilder.AddAction(FunctionSchemaAction);

}

//添加右键菜单的操作列表，意思是可以添加像：如果右点击到Pin上打开的菜单， 如果右点击到Node上打开的菜单等
void UScenarioGraphSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode,
	const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
{
	check(CurrentGraph);

	//如果右键点击到节点上
	if (InGraphNode != nullptr)
	{
		MenuBuilder->BeginSection(FName("ScenarioNodeMenu"), LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		{
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Delete);
			MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().ReconstructNodes);
			MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
		}
		MenuBuilder->EndSection();
	}
	Super::GetContextMenuActions(CurrentGraph, InGraphNode, InGraphPin, MenuBuilder, bIsDebugging);
}

/* 判断两个引脚是否可以链接 */
const FPinConnectionResponse UScenarioGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if(A->GetOwningNode() == B->GetOwningNode())
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("试试中文，这是同一个节点！"));
	//if(A->PinType.PinSubCategory == FScenarioPinUtil::SubPinCategoryRoot && B->PinType.PinSubCategory == FScenarioPinUtil::SubPinCategoryEnd)
	//	return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_DISALLOW, TEXT("不允许"));
	//else if (B->PinType.PinSubCategory == FScenarioPinUtil::SubPinCategoryRoot && A->PinType.PinSubCategory == FScenarioPinUtil::SubPinCategoryEnd)
	//	return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_DISALLOW, TEXT("引脚不允许连接"));
	//如果A是允许多个的节点，B是允许一个的节点
	else if( A->PinType.PinCategory == FScenarioPinUtil::PinCategoryMulti && B->PinType.PinCategory == FScenarioPinUtil::PinCategoryNormal )
		return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT(""));
	//如果A是允许一个的节点，B是允许多个的节点
	else if (A->PinType.PinCategory == FScenarioPinUtil::PinCategoryNormal && B->PinType.PinCategory == FScenarioPinUtil::PinCategoryMulti)
		return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
	//如果AB都是单个节点
	else if (A->PinType.PinCategory == FScenarioPinUtil::PinCategoryNormal && B->PinType.PinCategory == FScenarioPinUtil::PinCategoryNormal)
		return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_BREAK_OTHERS_AB , TEXT(""));
	//如果AB都是多个节点
	else if (A->PinType.PinCategory == FScenarioPinUtil::PinCategoryMulti && B->PinType.PinCategory == FScenarioPinUtil::PinCategoryMulti)
		//return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE, TEXT(""));
		return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, TEXT("创建带有条件的连接"));
	//如果A是随意PIN，B是需要断开所有A的引脚
	//else if(B->PinType.PinCategory == FScenarioPinUtil::PinInputBreakSourceOutput)
		//return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT("不允许在装配节点上进行分支"));

	//如果不允许连接
	else if (A->PinType.PinCategory == FScenarioPinUtil::PinCategoryNotAllow || B->PinType.PinCategory == FScenarioPinUtil::PinCategoryNotAllow)
		return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_DISALLOW, TEXT("引脚不允许连接"));
	else
		return FPinConnectionResponse(ECanCreateConnectionResponse::CONNECT_RESPONSE_DISALLOW, TEXT("节点啥也不是！"));

}

/*链接两个引脚并通过中间节点 可以在此自定义中间节点*/
bool UScenarioGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	//UE_LOG(LogTemp, Warning, TEXT(" $#### CreateAutomaticConversionNodeAndConnections"));
	UScenarioNodeNormal* NodeA = Cast<UScenarioNodeNormal>(A->GetOwningNode());
	UScenarioNodeNormal* NodeB = Cast<UScenarioNodeNormal>(B->GetOwningNode());

	FName TransName("NewCondition");

	if ((NodeA != NULL) && (NodeB != NULL) && (NodeA->GetOutPutPin() != NULL) && (NodeB->GetInPutPin() != NULL))
		//&& (NodeA->GetInPutPin() != NULL) && (NodeA->GetOutPutPin() != NULL)
		//&& (NodeB->GetInPutPin() != NULL) && (NodeB->GetOutPutPin() != NULL))
	{
		//先创建一个中间节点，再将其连接
		UConditionConversionNodeBase* ConversionNode = FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UConditionConversionNodeBase>( NodeA->GetGraph(),
			NewObject<UConditionConversionNodeBase>(), FVector2D(0.0f, 0.0f) ,false);
		if (ConversionNode != nullptr)
		{
			if (A->Direction == EGPD_Output)
			{
				return ConversionNode->CreateConnections(NodeA, NodeB, TransName);
			}
			else
			{
				return ConversionNode->CreateConnections(NodeB, NodeA, TransName);
			}

		}
	}

	return false;
}

/* 链接两个引脚 */
bool UScenarioGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	//因为其实只写了RightNodeBox，所以要判断下，响应点击的引脚应该都是OutPut
	if (A->Direction == B->Direction)
		if (UScenarioNodeNormal* Node = Cast<UScenarioNodeNormal>(B->GetOwningNode()))
		{
			if (A->Direction == EGPD_Input)
			{
				B = Node->GetOutPutPin();
			}
			else
			{
				B = Node->GetInPutPin();
			}
		}

	const bool bModified = UEdGraphSchema::TryCreateConnection(A, B);

	return bModified;
}

void UScenarioGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("FSMAssetEditorNativeNames", "GraphEd_BreakNodeLinks", "Break Node Links"));

	Super::BreakNodeLinks(TargetNode);
}

void UScenarioGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("FSMAssetEditorNativeNames", "GraphEd_BreakPinLinks", "Break Pin Links"));

	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
}

void UScenarioGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("FSMAssetEditorNativeNames", "GraphEd_BreakSinglePinLink", "Break Pin Link"));

	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

#undef LOCTEXT_NAMESPACE