#include "ConditionConversionNodeBase.h"
#include "Runtime/SlateCore/Public/Widgets/SBoxPanel.h"
#include "ScenarioGraph.h"
#include "ScenarioPin.h"
#include "ScenarioEditor.h"
#include "Editor/GraphEditor/Public/SGraphPanel.h"
#include "Runtime/Slate/Public/Widgets/Text/SInlineEditableTextBlock.h"
#include "Editor/GraphEditor/Public/ConnectionDrawingPolicy.h"
#include "ScenarioNodeNormal.h"

const int32 UConditionConversionNodeBase::INPUT_PIN_INDEX = 0;
const int32 UConditionConversionNodeBase::OUTPUT_PIN_INDEX = 1;

#define LOCTEXT_NAMESPACE "SConditionConversionNodeBase"

/************************************************************************/
/*	                   SScenarioNodeNormal                               */
/************************************************************************/
void SConditionConversionNodeBase::Construct(const FArguments& InArgs, UConditionConversionNodeBase* InNode)
{
	//设置鼠标Hover的时候的显示
	//SetCursor(EMouseCursor::CardinalCross);

	GraphNode = InNode;


	//更新节点
	UpdateGraphNode();
}

void SConditionConversionNodeBase::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	FLinearColor TitleShadowColor(0.4f, 0.4f, 0.4f);
	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	/*this->GetOrAddSlot(ENodeZone::BottomCenter)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			// STATE NAME AREA
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(0.0f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
				.BorderBackgroundColor(TitleShadowColor)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Visibility(EVisibility::SelfHitTestInvisible)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						// POPUP ERROR MESSAGE
						SAssignNew(ErrorText, SErrorText)
						.BackgroundColor(this, &SConditionConversionNodeBase::GetErrorColor)
						.ToolTipText(this, &SConditionConversionNodeBase::GetErrorMsgToolTip)
					]
					+ SHorizontalBox::Slot()
					.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(InlineEditableText, SInlineEditableTextBlock)
							.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
							.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
							.OnVerifyTextChanged(this, &SConditionConversionNodeBase::OnVerifyNameTextChanged)
							.OnTextCommitted(this, &SConditionConversionNodeBase::OnNameTextCommited)
							.IsReadOnly(this, &SConditionConversionNodeBase::IsNameReadOnly)
							.IsSelected(this, &SConditionConversionNodeBase::IsSelectedExclusively)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							NodeTitle.ToSharedRef()
						]
					]
				]
			]
		];*/


	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0)
			//.BorderBackgroundColor(this, &SGraphNodeAnimState::GetBorderBackgroundColor)
			.BorderBackgroundColor(FLinearColor::Gray)
			[
				SNew(SOverlay)
				//PIN AREA,并且把生成的SVerticalBox付给RightNodeBox
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
					.BorderBackgroundColor(TitleShadowColor)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(NodeNameBlock , STextBlock)
						.Text(FText::FromString(L"条件"))
					]
				]
			]
		];
}

void SConditionConversionNodeBase::UpdateNodeNmae(FString NodeName)
{
	NodeNameBlock->SetText(NodeName);
}

void SConditionConversionNodeBase::MoveTo(const FVector2D & NewPosition, FNodeSet & NodeFilter)
{
}

/* 根据其余节点位置，做第二遍布局 */
void SConditionConversionNodeBase::PerformSecondPassLayout(const TMap<UObject *, TSharedRef<SNode>>& InNodeToWidgetLookup) const
{
	UConditionConversionNodeBase* TransNode = CastChecked<UConditionConversionNodeBase>(GraphNode);

	// Find the geometry of the state nodes we're connecting
	FGeometry StartGeom;
	FGeometry EndGeom;

	int32 TransIndex = 0;
	int32 NumOfTrans = 1;

	UScenarioNodeNormal* PrevState = TransNode->GetFromNode();
	UScenarioNodeNormal* NextState = TransNode->GetTargetNode();
	if ((PrevState != NULL) && (NextState != NULL))
	{
		const TSharedRef<SNode>* pPrevNodeWidget = InNodeToWidgetLookup.Find(PrevState);
		const TSharedRef<SNode>* pNextNodeWidget = InNodeToWidgetLookup.Find(NextState);
		if ((pPrevNodeWidget != NULL) && (pNextNodeWidget != NULL))
		{
			const TSharedRef<SNode>& PrevNodeWidget = *pPrevNodeWidget;
			const TSharedRef<SNode>& NextNodeWidget = *pNextNodeWidget;

			StartGeom = FGeometry(FVector2D(PrevState->NodePosX, PrevState->NodePosY), FVector2D::ZeroVector, PrevNodeWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(NextState->NodePosX, NextState->NodePosY), FVector2D::ZeroVector, NextNodeWidget->GetDesiredSize(), 1.0f);

			TArray<UConditionConversionNodeBase*> Transitions;

			//Get all transitions linked PrevState->GetOutputPin()
			for (int32 LinkIndex = 0; LinkIndex < PrevState->GetOutPutPin()->LinkedTo.Num(); ++LinkIndex)
			{
				UEdGraphNode* TargetNode = PrevState->GetOutPutPin()->LinkedTo[LinkIndex]->GetOwningNode();
				if (UConditionConversionNodeBase* Transition = Cast<UConditionConversionNodeBase>(TargetNode))
				{
					Transitions.Add(Transition);
				}
			}

			Transitions = Transitions.FilterByPredicate([NextState](const UConditionConversionNodeBase* InTransition) -> bool
			{
				return InTransition->GetTargetNode() == NextState;
			});

			TransIndex = Transitions.IndexOfByKey(TransNode);
			NumOfTrans = Transitions.Num();
		}
	}

	//Position Node
	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, TransIndex, NumOfTrans);


}

void SConditionConversionNodeBase::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	const float Height = 30.0f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	// Calculate node offset in the case of multiple transitions between the same two nodes
	// MultiNodeOffset: the offset where 0 is the centre of the transition, -1 is 1 <size of node>
	// towards the PrevStateNode and +1 is 1 <size of node> towards the NextStateNode.

	const float MutliNodeSpace = 0.2f; // Space between multiple transition nodes (in units of <size of node> )
	const float MultiNodeStep = (1.f + MutliNodeSpace); //Step between node centres (Size of node + size of node spacer)

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

FSlateColor SConditionConversionNodeBase::GetTransitionColor() const
{
	const FLinearColor HoverColor(0.724f, 0.256f, 0.0f, 1.0f);
	FLinearColor BaseColor(0.9f, 0.9f, 0.9f, 1.0f);
	return IsHovered() ? HoverColor : BaseColor;
}

void SConditionConversionNodeBase::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UConditionConversionNodeBase* TransNode = CastChecked<UConditionConversionNodeBase>(GraphNode);
	if (UEdGraphPin* Pin = TransNode->GetInPutPin())
	{
		GetOwnerPanel()->AddPinToHoverSet(Pin);
	}

	SGraphNode::OnMouseEnter(MyGeometry, MouseEvent);
}

void SConditionConversionNodeBase::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	UConditionConversionNodeBase* TransNode = CastChecked<UConditionConversionNodeBase>(GraphNode);
	if (UEdGraphPin* Pin = TransNode->GetInPutPin())
	{
		GetOwnerPanel()->RemovePinFromHoverSet(Pin);
	}

	SGraphNode::OnMouseLeave(MouseEvent);
}

/************************************************************************/
/*	                   UConditionConversionNodeBase                               */
/************************************************************************/

UConditionConversionNodeBase::UConditionConversionNodeBase()
{

}

//从Detail中取名字也可以！
FText UConditionConversionNodeBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(NodeName);
}

FLinearColor UConditionConversionNodeBase::GetNodeTitleColor() const
{
	return FLinearColor::Green;
}

void UConditionConversionNodeBase::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Input, FScenarioPinUtil::PinCategoryConnection, TEXT("In"));
	CreatePin(EEdGraphPinDirection::EGPD_Output, FScenarioPinUtil::PinCategoryConnection, TEXT("Out"));
}

/* 一旦链接关系发生变化，直接清除该节点 */
void UConditionConversionNodeBase::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		// 转换节点永远都应该有输入和输出链接
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}


/*删除节点的众多操作*/
void UConditionConversionNodeBase::DestroyNode()
{

	Super::DestroyNode();
}

void UConditionConversionNodeBase::OnRenameNode(const FString& NewName)
{
	NodeName = NodeName;

	//再更新一下节点,调用下EDGraph的Modify();
	UScenarioGraph* CurGraph = Cast< UScenarioGraph >(GetGraph());
	if (CurGraph != nullptr)
	{
		CurGraph->Modify();
	}
}

UEdGraphPin* UConditionConversionNodeBase::GetOutPutPin()
{
	return Pins[OUTPUT_PIN_INDEX];
}
#pragma optimize("",off)
UEdGraphPin* UConditionConversionNodeBase::GetInPutPin()
{
	return Pins[INPUT_PIN_INDEX];
}
#pragma optimize("",on)

TSharedPtr<SGraphNode> UConditionConversionNodeBase::CreateVisualWidget()
{
	SAssignNew(SNodeWidgetShared, SConditionConversionNodeBase, this);

	return SNodeWidgetShared;//SAssignNew(SNodeWidgetShared,SScenarioNodeNormal,this);
}

//双击节点的调用函数
void UConditionConversionNodeBase::DoubleClickNodeCall()
{

	//(SNodeWidgetShared.Get())->UpdateNodeNmae("I HAVE BEEN DOUBLE");

	//双击打开一个Tab
	/*if (bOpenGraph)
	{
		if (UScenarioGraph* Graph = Cast<UScenarioGraph>(GetGraph()))
		{
			if (Graph->ScenarioEditor)
				Graph->ScenarioEditor->OpenInsideNodeGraphTab( );
		}
	}*/
}

//当Detail属性变化时更新 可能之后的很多操作都在这儿了
void UConditionConversionNodeBase::OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent)
{
	(SNodeWidgetShared.Get())->UpdateNodeNmae(NodeName);
}


/* 获取OutPin连接的所有Pin */
TArray<UEdGraphPin*> UConditionConversionNodeBase::GetAllOutPinLinked()
{
	TArray<UEdGraphPin*> NoneArray;

	UEdGraphPin* OutPin = GetOutPutPin();
	if (OutPin)
		return OutPin->LinkedTo;

	return NoneArray;
}

/* 获取该Node OutpUT链接的所有Node */
TArray<UEdGraphNode*> UConditionConversionNodeBase::GetAllOutNodeLinked()
{
	TArray<UEdGraphNode*> OutNode;

	UEdGraphPin* OutPin = GetOutPutPin();
	if (OutPin)
	{
		TArray<UEdGraphPin*> OutPins = OutPin->LinkedTo;
		if (OutPins.Num() > 0)
		{
			for (TArray<UEdGraphPin*>::TConstIterator iter = OutPins.CreateConstIterator(); iter; ++iter)
			{
				if (UEdGraphPin* CurPin = *iter)
				{
					OutNode.Add(CurPin->GetOwningNode());
				}
			}
		}
	}
	return OutNode;

}

/* 获取该Node Input的所有Node */
TArray<UEdGraphNode*> UConditionConversionNodeBase::GetAllInputNodeLinked()
{
	TArray<UEdGraphNode*> InputNode;

	UEdGraphPin* InputPin = GetInPutPin();
	if (InputPin)
	{
		TArray<UEdGraphPin*> InputPins = InputPin->LinkedTo;
		if (InputPins.Num() > 0)
		{
			for (TArray<UEdGraphPin*>::TConstIterator iter = InputPins.CreateConstIterator(); iter ; ++iter)
			{
				if (UEdGraphPin* CurPin = *iter)
				{
					InputNode.Add(CurPin->GetOwningNode());
				}
			}
		}
	}

	return InputNode;
}

UScenarioNodeNormal * UConditionConversionNodeBase::GetFromNode() const
{
	return FromNode;
}

UScenarioNodeNormal * UConditionConversionNodeBase::GetTargetNode() const
{
	return TargetNode;
}

void UConditionConversionNodeBase::SetTransitionName(FName NewName)
{
	Transition = NewName;
}

FName UConditionConversionNodeBase::GetTransitionName() const
{
	return Transition;
}

bool UConditionConversionNodeBase::CreateConnections(class UScenarioNodeNormal* PreviousState, class UScenarioNodeNormal* NextState, FName TransName)
{
	FromNode = PreviousState;
	TargetNode = NextState;

	SetTransitionName(TransName);

	const FScopedTransaction Transaction(LOCTEXT("EditorNewTransition", "FSM Editor: New Transition"));

	GetInPutPin()->Modify();
	GetInPutPin()->LinkedTo.Empty();

	PreviousState->GetOutPutPin()->Modify();
	PreviousState->GetOutPutPin()->MakeLinkTo(GetInPutPin());

	// This to next
	GetOutPutPin()->Modify();
	GetOutPutPin()->LinkedTo.Empty();

	NextState->GetInPutPin()->Modify();
	GetOutPutPin()->MakeLinkTo(NextState->GetInPutPin());

	return true;
}


#undef LOCTEXT_NAMESPACE