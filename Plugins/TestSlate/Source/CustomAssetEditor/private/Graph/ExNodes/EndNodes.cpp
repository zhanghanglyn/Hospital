#include "EndNodes.h"
#include "ScenarioGraph.h"
#include "ScenarioPin.h"
#include "ScenarioEditor.h"
#include "Runtime/SlateCore/Public/Widgets/SBoxPanel.h"
#include "Runtime/Slate/Public/Widgets/Text/SInlineEditableTextBlock.h"

UEndNodes::UEndNodes()
{
	NodeName = "End";
	bOpenGraph = false;
	NodeCategory = FScenarioNodeUtil::NodeCategoryEnd;
}

void UEndNodes::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Input, FScenarioPinUtil::PinCategoryMulti,TEXT("IN"));
	//CreatePin(EEdGraphPinDirection::EGPD_Output, FScenarioPinUtil::PinCategoryNotAllow, TEXT("OUT"));
}

TSharedPtr<SGraphNode> UEndNodes::CreateVisualWidget()
{
	SAssignNew(SNodeWidgetShared, SScenarioEndNode, this).NodeBgColor(FLinearColor::Red);

	return SNodeWidgetShared;//SAssignNew(SNodeWidgetShared,SScenarioNodeNormal,this);
}

#pragma optimize("",off)
UEdGraphPin* UEndNodes::GetInPutPin()
{
	return Pins[0];
}
#pragma optimize("",on)
/************************************************************************/
/*	                   SScenarioEndNode                               */
/************************************************************************/
void SScenarioEndNode::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	//�������Hover��ʱ�����ʾ
	SetCursor(EMouseCursor::CardinalCross);

	GraphNode = InNode;
	OwnerGraphNode = Cast<UEndNodes>(InNode);

	NodeBgColor = InArgs._NodeBgColor;

	//���½ڵ�
	UpdateGraphNode();
}

void SScenarioEndNode::UpdateGraphNode()
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
				//PIN AREA,���Ұ����ɵ�SVerticalBox����RightNodeBox
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(LeftNodeBox, SVerticalBox)  //��Ȧ�������ҵ�RightNodeBox
				]

				//��������
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
					.Visibility(EVisibility::SelfHitTestInvisible)  //
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							// POPUP ERROR MESSAGE
							SAssignNew(ErrorText, SErrorText)
							.BackgroundColor(this, &SScenarioEndNode::GetErrorColor)	//�д����ʱ���ȥȡ��ɫ
							.ToolTipText(this, &SScenarioEndNode::GetErrorMsgToolTip)
						]
						//�������򣬿ɵ������
						+SHorizontalBox::Slot()
						.Padding(FMargin(20.0f, 0.0f, 20.0f, 0.0f))
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SAssignNew(InlineEditableText, SInlineEditableTextBlock)
								.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
								.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
								.OnVerifyTextChanged(this, &SScenarioEndNode::OnVerifyNameTextChanged)
								.OnTextCommitted(this, &SScenarioEndNode::OnNameTextCommited)
								.IsReadOnly(this, &SScenarioEndNode::IsNameReadOnly)
								.IsSelected(this, &SScenarioEndNode::IsSelectedExclusively)
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								NodeTitle.ToSharedRef()
							]
						]
					]
				]

			]
		];

	//����¶������Widget������
	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);

	CreatePinWidgets();
}

/*  */
void SScenarioEndNode::CreatePinWidgets()
{

	UEdGraphPin* CurPin = OwnerGraphNode->GetInPutPin();
	if (CurPin)
	{
		TSharedPtr< SScenarioPin> NewPin = SNew(SScenarioPin, CurPin);
		check(NewPin.IsValid());

		this->AddPin(NewPin.ToSharedRef());
	}
}

/* ��Ϊ��Ȧ�����ҵ�RightNode~���� Input��OutPut�Ͷ�����OutPut�� */
void SScenarioEndNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	LeftNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];

	OutputPins.Add(PinToAdd);   //��������OutPutPins�б���
}