
#include "TreeNode.h"

STreeNode::STreeNode()
	: BgImageBrush(FCoreStyle::Get().GetBrush("Border"))
{

}

void STreeNode::Construct(const FArguments& InArgs, SCanvasTree* Tree)
{
	BgImageBrush = InArgs._ABgImageBrush;

	bFlipForRightToLeftFlowDirection = InArgs._AbFlipForRightToLeftFlowDirection.Get();

	BorderBackgroundColor = InArgs._ABorderBackgroundColor;

	bIsFocusable = InArgs._bIsFocusable.Get();

	ClickNodeCallDelegate = InArgs._ClickNodeCallBack;

	UpNodeCallDelegate = InArgs._UpNodeCallBack;

	DeleteNodeCallDelegate = InArgs._DeleteNodeCallBack;

	MoveNodeCallDelegate = InArgs._MoveNodeCallBack;

	//����Tree��ָ��
	CanvasTree = Tree;
	ChildNode = nullptr;
	ParentNode = nullptr;

	//19.11.04 ����ʱ������Ƴߴ�
	DesiredSizeScale = FVector2D(100, 60);
	//���õ������
	MoveRect = new A_Rect();

	MoveRect->left = LineRange;
	MoveRect->right = DesiredSizeScale.Get().X - LineRange;
	MoveRect->top = LineRange;
	MoveRect->bottom = DesiredSizeScale.Get().Y - LineRange;

	//�������ߵ�Ϊ�ؼ�����
	CenterPosition = FVector2D(DesiredSizeScale.Get().X / 2, DesiredSizeScale.Get().Y / 2);

	this->ChildSlot
	//.HAlign(InArgs._HAlign)
	//.VAlign(InArgs._VAlign)
	//.Padding(InArgs._Padding)
	[
		InArgs._Content.Widget
	];
}

int32 STreeNode::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{

 	const FSlateBrush* BrushResource = BgImageBrush.Get();
	const bool bEnabled = ShouldBeEnabled(bParentEnabled);

	if (BrushResource && BrushResource->DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		const bool bShowDisabledEffect = true;  //��Ĭ��������ʾ��ʧ��Ч
		const ESlateDrawEffect DrawEffects = (bShowDisabledEffect && !bEnabled) ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;


		//�ڻ����б������һ������
		if (bFlipForRightToLeftFlowDirection && GSlateFlowDirection == EFlowDirection::RightToLeft)
		{
			const FGeometry FlippedGeometry = AllottedGeometry.MakeChild(FSlateRenderTransform(FScale2D(-1, 1)));
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				FlippedGeometry.ToPaintGeometry(),
				BrushResource,
				DrawEffects,
				BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * BorderBackgroundColor.Get().GetColor(InWidgetStyle)
			);
		}
		else
		{
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				BrushResource,
				DrawEffects,
				BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * BorderBackgroundColor.Get().GetColor(InWidgetStyle)
			);
		}
	}
	
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bEnabled);

}

FVector2D STreeNode::ComputeDesiredSize(float LayoutScaleMultiplier) const
{

	//return DesiredSizeScale.Get() * SCompoundWidget::ComputeDesiredSize(LayoutScaleMultiplier);
	return DesiredSizeScale.Get();
}

void STreeNode::SetContent(TSharedRef< SWidget > InContent)
{
	ChildSlot
	[
		InContent
	];
}

const TSharedRef< SWidget >& STreeNode::GetContent() const
{
	return ChildSlot.GetWidget();
}

void STreeNode::ClearContent()
{
	ChildSlot.DetachWidget();
}

void STreeNode::SetBgImage(const TAttribute<const FSlateBrush*>& InBorderImage)
{
	if (!BgImageBrush.IdenticalTo(InBorderImage))
	{
		BgImageBrush = InBorderImage;
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
}

void STreeNode::SetBorderBackgroundColor(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	if (!BorderBackgroundColor.IdenticalTo(InColorAndOpacity))
	{
		BorderBackgroundColor = InColorAndOpacity;
		Invalidate(EInvalidateWidget::PaintAndVolatility);
	}
}

/************************************************************************/
/*                     ������¼����                            */
/************************************************************************/
bool STreeNode::SupportsKeyboardFocus() const
{
	// Buttons are focusable by default
	return bIsFocusable;
}

void STreeNode::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SCompoundWidget::OnFocusLost(InFocusEvent);

	//Release();
}

bool STreeNode::IsInteractable() const
{
	return true;
}

/*
�����ʱ����¼һ������״̬���������ϲ㷢�ͣ���ʼ������ͷ
�����������ʱ�����ߣ�����Ǹ�������ʱ�������϶���
*/
#pragma  optimize("", off)
FReply STreeNode::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();

	//�ж��¿ؼ��ɼ�����������갴�»����ǵ���¼�ʱ��������
	if (IsEnabled() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent())
	{
		PressedScreenSpacePosition = MouseEvent.GetScreenSpacePosition();
		FVector2D absolutePos = MyGeometry.GetAbsolutePosition();
		FVector2D ClickSizeInNode = PressedScreenSpacePosition - absolutePos;

		//����Line
		if (CheckClickPosType(ClickSizeInNode) == ClickType::CLICK_LINE)
		{
			BNodeMove = false;
			SetAbsoluteCenterLinePos(absolutePos);

			//PressFunction(ClickSizeInNode);
		}
		else//�ƶ�������
		{
			MouseClickPos = PressedScreenSpacePosition;
			BNodeMove = true;
		}

		PressFunction(ClickSizeInNode);


		//����ð�ݣ����ܻ�ȡ��ȷ�ĵ��λ��
		Reply = FReply::Unhandled();
	}
	//����ɾ��
	else if (IsEnabled() && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton || MouseEvent.IsTouchEvent())
	{
		DeleteNodeCallDelegate.ExecuteIfBound(this);
		Reply = FReply::Handled();
	}
	Invalidate(EInvalidateWidget::Layout);

	return Reply;
}
#pragma  optimize("", on)

void STreeNode::PressFunction(FVector2D AbsolutePos)
{
	//�����ص�
	ClickNodeCallDelegate.ExecuteIfBound(AbsolutePos,this);
}

/* �ƶ�ʱ���ж��£�����������ߵ������ƶ����������������
*/
#pragma  optimize("", off)
FReply STreeNode::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();

	if (IsEnabled() && BNodeMove == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Moving!!!"));
	
		//�����ƶ���ĵ�ǰλ��
		PressedScreenSpacePosition = MouseEvent.GetScreenSpacePosition();
		FVector2D absolutePos = MyGeometry.GetAbsolutePosition();
		FVector2D ClickSizeInNode = PressedScreenSpacePosition - absolutePos;

		/* 19.11.20 */
		SetAbsoluteCenterLinePos(absolutePos);

		//���ݵ����λ�ã���һ��OFFSET
		FVector2D offsetPos = -1 * (MouseClickPos - PressedScreenSpacePosition);

		MoveNodeCallDelegate.ExecuteIfBound(this , offsetPos);
	}
	
	return Reply;
}
#pragma  optimize("", on)

/* ���̧��ʱʱ�ж��Ƿ�ǰTree��ѡ�нڵ㣬����У�
	�жϵ�ǰNODE�Ƿ�ΪTree��ѡ�нڵ㣬����ǣ�������������ǣ�
	�����Լ��ĸ��ڵ�ΪTree�ĵ�ǰѡ�нڵ�,���õ�ǰѡ�нڵ���ӽڵ�Ϊ����
*/
FReply STreeNode::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();

	//�ж��¿ؼ��ɼ�����������갴�»����ǵ���¼�ʱ��������
	if (IsEnabled() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent())
	{
		PressedScreenSpacePosition = MouseEvent.GetScreenSpacePosition();
		FVector2D absolutePos = MyGeometry.GetAbsolutePosition();
		FVector2D ClickSizeInNode = PressedScreenSpacePosition - absolutePos;

		SetAbsoluteCenterLinePos(absolutePos);

		UpNodeCallDelegate.ExecuteIfBound(ClickSizeInNode, this);
	}

	//����ƶ���ʶ
	BNodeMove = false;

	return Reply;
}

void STreeNode::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	
}


void STreeNode::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	BNodeMove = false;
}

/*�ȼ������м�����Ȼ���жϵ�����Ƿ����м�����
*/
STreeNode::ClickType STreeNode::CheckClickPosType(FVector2D ClickPos)
{
	if (ClickPos.X < MoveRect->right && ClickPos.X > MoveRect->left)
	{
		if (ClickPos.Y < MoveRect->bottom && ClickPos.Y > MoveRect->top)
			return ClickType::CLICK_MOVE;
	}
	return ClickType::CLICK_LINE;
}

/************************************************************************/
/*						         END							        */
/************************************************************************/
void STreeNode::SetAbsoluteCenterLinePos(FVector2D NodeStartPos)
{
	LinePos =(NodeStartPos + CenterPosition);
	UE_LOG(LogTemp, Warning, TEXT("Pos!!! : %f + %f"), M_NodeData->LinePos.X, M_NodeData->LinePos.Y);
}
FVector2D STreeNode::GetAbsoluteCenterLinePos()
{
	return LinePos;
}

void STreeNode::SetChildNode(STreeNode* _ChildNode)
{
	ChildNode = _ChildNode;
	M_NodeData->ChildID = _ChildNode->M_NodeData->DataID;
}

void STreeNode::SetParentNode(STreeNode* _ParentNode)
{
	ParentNode = _ParentNode;
	M_NodeData->ParentID = _ParentNode->M_NodeData->DataID;
}

STreeNode* STreeNode::GetChildNode()
{
	return ChildNode;
}

STreeNode* STreeNode::GetParentNode()
{
	return ParentNode;
}

/*********************
	�������	
*/
void STreeNode::SetNodeData(NodeData* _NodeData)
{
	M_NodeData = new NodeData(_NodeData);

}

void STreeNode::SetNodeLinePos(FVector2D _LinePos)
{
	M_NodeData->LinePos = _LinePos;
}

void STreeNode::SetChildAID(int32 _AID)
{
	M_NodeData->C_AID = _AID;
}

int32 STreeNode::GetChildAID()
{
	return M_NodeData->C_AID;
}

void STreeNode::SetParentAID(int32 _AID)
{
	M_NodeData->P_AID = _AID;
}

int32 STreeNode::GetParentAID()
{
	return M_NodeData->P_AID;
}

void STreeNode::ClearParentNode()
{
	M_NodeData->ParentID = -1;
	ParentNode = nullptr;
}

void STreeNode::ClearChildNode()
{
	M_NodeData->ChildID = -1;
	ChildNode = nullptr;
}

void STreeNode::SetNodePos(FVector2D _NodePos)
{
	M_NodeData->Pos = FVector2D(_NodePos);

}