#include "ScenarioNodeNormal.h"
#include "Runtime/SlateCore/Public/Widgets/SBoxPanel.h"
#include "ScenarioGraph.h"
#include "ScenarioPin.h"
#include "ScenarioEditor.h"
#include "ConditionConversionNodeBase.h"
#include "Runtime/Slate/Public/Widgets/Text/SInlineEditableTextBlock.h"

const int32 UScenarioNodeNormal::INPUT_PIN_INDEX = 0;
const int32 UScenarioNodeNormal::OUTPUT_PIN_INDEX = 1;

#define LOCTEXT_NAMESPACE "ScenarioNodeNormal"

/************************************************************************/
/*	                   SScenarioNodeNormal                               */
/************************************************************************/
void SScenarioNodeNormal::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	//设置鼠标Hover的时候的显示
	SetCursor(EMouseCursor::CardinalCross);

	GraphNode = InNode;
	OwnerGraphNode = Cast<UScenarioNodeNormal>(InNode);

	NodeBgColor = InArgs._NodeBgColor;
	NodeTitleName = InArgs._CategoryTEXT.Get();
	BNeedCategoryTitle = InArgs._BUseCategoryTitle.Get();

	//更新节点
	UpdateGraphNode();
}

void SScenarioNodeNormal::UpdateGraphNode()
{
	InputPins.Empty();
	InputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	FLinearColor TitleShadowColor( 0.6f ,0.6f , 0.6f);
	TSharedPtr<SErrorText> ErrorText;			
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0)
			//.BorderBackgroundColor(this, &SGraphNodeAnimState::GetBorderBackgroundColor)
			.BorderBackgroundColor(NodeBgColor.Get()) //FLinearColor::Gray)
			[
				SNew(SOverlay)
				//PIN AREA,并且把生成的SVerticalBox付给RightNodeBox
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(RightNodeBox, SVerticalBox)  //整圈！都是我的RightNodeBox
				]

				//名字区域
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(10.0f)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
					.BorderBackgroundColor(TitleShadowColor)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					//.Visibility(EVisibility::SelfHitTestInvisible)  //
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							// POPUP ERROR MESSAGE
							SAssignNew(ErrorText, SErrorText)
							.BackgroundColor(this, &SScenarioNodeNormal::GetErrorColor)	//有错误的时候会去取颜色
							.ToolTipText(this, &SScenarioNodeNormal::GetErrorMsgToolTip)
						]
						//名字区域，可点击名字
						+SHorizontalBox::Slot()
						.Padding(FMargin(20.0f, 0.0f, 20.0f, 0.0f))
						[
							//加一个类型标题
							SAssignNew(CategoryTitleCtl, SVerticalBox)
							/*+ SVerticalBox::Slot()
							.Padding(10,2,10,2)
							.AutoHeight()
							.HAlign(HAlign_Center)
							[
								SNew(SBorder)
								.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
								.BorderBackgroundColor(FLinearColor::Black)
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text( NodeTitleName)
								]
							]*/
							+ SVerticalBox::Slot()
							.AutoHeight()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SAssignNew(InlineEditableText, SInlineEditableTextBlock)
								.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
								.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
								.OnVerifyTextChanged(this, &SScenarioNodeNormal::OnVerifyNameTextChanged)
								.OnTextCommitted(this, &SScenarioNodeNormal::OnNameTextCommited)
								.IsReadOnly(this, &SScenarioNodeNormal::IsNameReadOnly)
								.IsSelected(this, &SScenarioNodeNormal::IsSelectedExclusively)
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								NodeTitle.ToSharedRef()
								
							]
						]
					]
				]

			]
		];
	if (BNeedCategoryTitle)
	{
		CategoryTitleCtl->AddSlot()
			.Padding(10, 2, 10, 2)
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
			.BorderBackgroundColor(FLinearColor::Black)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(NodeTitleName)
			]
			];
	}


	//将暴露出来的Widget存起来
	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);

	CreatePinWidgets();

}

/*  */
void SScenarioNodeNormal::CreatePinWidgets()
{
	
	UEdGraphPin* CurPin = OwnerGraphNode->GetOutPutPin();
	if (CurPin)
	{
		TSharedPtr< SScenarioPin> NewPin = SNew(SScenarioPin, CurPin);
		check(NewPin.IsValid());

		this->AddPin(NewPin.ToSharedRef());
	}
}

/* 因为整圈都是我的RightNode~所以 Input和OutPut就都放在OutPut啦 */
void SScenarioNodeNormal::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	RightNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];

	OutputPins.Add(PinToAdd);   //放入所有OutPutPins列表中
}

//外部方法相关
void SScenarioNodeNormal::UpdateNodeNmae(FString NodeName)
{
	InlineEditableText->SetText(NodeName);
}

FReply SScenarioNodeNormal::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	//会调用自身UNode的双击方法！
	OwnerGraphNode->DoubleClickNodeCall();

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		OnDoubleClick.ExecuteIfBound(GraphNode);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

/************************************************************************/
/*	                   UScenarioNodeNormal                               */
/************************************************************************/

UScenarioNodeNormal::UScenarioNodeNormal()
{

}

//从Detail中取名字也可以！
FText UScenarioNodeNormal::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(NodeName);
}

void UScenarioNodeNormal::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Input, FScenarioPinUtil::PinCategoryMulti, TEXT("In"));
	CreatePin(EEdGraphPinDirection::EGPD_Output, FScenarioPinUtil::PinCategoryMulti, TEXT("Out"));
}

void UScenarioNodeNormal::OnRenameNode(const FString& NewName)
{
	NodeName = NodeName;

	//再更新一下节点,调用下EDGraph的Modify();
	UScenarioGraph* CurGraph = Cast< UScenarioGraph >(GetGraph());
	if (CurGraph != nullptr)
	{
		CurGraph->Modify();
	}
}

UEdGraphPin* UScenarioNodeNormal::GetOutPutPin()
{
	return Pins[OUTPUT_PIN_INDEX];
}
#pragma optimize("",off)
UEdGraphPin* UScenarioNodeNormal::GetInPutPin()
{
	return Pins[INPUT_PIN_INDEX];
}
#pragma optimize("",on)

TSharedPtr<SGraphNode> UScenarioNodeNormal::CreateVisualWidget()
{
	SAssignNew(SNodeWidgetShared, SScenarioNodeNormal, this);

	return SNodeWidgetShared;//SAssignNew(SNodeWidgetShared,SScenarioNodeNormal,this);
}

//双击节点的调用函数
void UScenarioNodeNormal::DoubleClickNodeCall()
{

	//(SNodeWidgetShared.Get())->UpdateNodeNmae("I HAVE BEEN DOUBLE");

	//双击打开一个Tab
	if (bOpenGraph)
	{
		if (UScenarioGraph* Graph = Cast<UScenarioGraph>(GetGraph()))
		{
			if (Graph->ScenarioEditor)
				Graph->ScenarioEditor->OpenInsideNodeGraphTab(this);
		}
	}
	
		

}

//当Detail属性变化时更新 可能之后的很多操作都在这儿了
void UScenarioNodeNormal::OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent)
{
	(SNodeWidgetShared.Get())->UpdateNodeNmae(NodeName);
}

/*删除节点的众多操作*/
void UScenarioNodeNormal::DestroyNode()
{

	UEdGraphNode::DestroyNode();
}

/* 获取OutPin连接的所有Pin */
TArray<UEdGraphPin*> UScenarioNodeNormal::GetAllOutPinLinked()
{
	TArray<UEdGraphPin*> NoneArray;

	UEdGraphPin* OutPin = GetOutPutPin();
	if (OutPin)
		return OutPin->LinkedTo;

	return NoneArray;
}

/* 获取该Node OutpUT链接的所有Node */
TArray<UEdGraphNode*> UScenarioNodeNormal::GetAllOutNodeLinked()
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
					//还要判断下是否是连接节点
					UConditionConversionNodeBase* TransNode = Cast<UConditionConversionNodeBase>(CurPin->GetOwningNode());
					if (TransNode == nullptr)
					{
						if (CurPin->GetOwningNode())
							OutNode.Add(CurPin->GetOwningNode());
					}	
					//如果是连接节点，取连接节点的下一个节点
					else
					{
						UScenarioNodeNormal* NodeNormal = TransNode->GetTargetNode();
						if (UEdGraphNode* NextNode = Cast<UEdGraphNode>(NodeNormal))
						{
							OutNode.Add(NextNode);
						}
					}
				}
			}
		}
	}
	return OutNode;

}


/* 获取该Node Input的所有Node */
TArray<UEdGraphNode*> UScenarioNodeNormal::GetAllInputNodeLinked()
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

#undef LOCTEXT_NAMESPACE