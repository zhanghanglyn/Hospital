#include "SLayoutTree.h"

#define  LOCTEXT_NAMESPACE "SCanvasTree"

SCanvasTree::SCanvasTree() : Children(this), ArrowChildren(this), BorderImg(FCoreStyle::Get().GetBrush("Border"))
{
	SetCanTick(false);
	bCanSupportFocus = false;
	
}

void SCanvasTree::Construct(const FArguments& InArgs, FVector2D Position ,FVector2D Size)
{

	M_Position = Position;
	M_SIZE = Size;
	BorderImg = InArgs._BorderBrush;

	NodeArray.Empty();

	//加个背景（边框在底下用来标示）
	SCanvasTree::FSlot& NewSlot = *new SCanvasTree::FSlot();
	NewSlot
	[
		SNew(SImage).Image(FTestSlateStyle::Get().GetBrush("UI.TreeBg"))
	];
	ArrowChildren.Add(&NewSlot);

	//TSharedPtr< STreeArrow> mTreeArrowPanel;
	//添加一个箭头处理层，在背景层之上
	SCanvasTree::FSlot& ArrowSlot = *new SCanvasTree::FSlot();
	ArrowSlot
	[
		SAssignNew(mTreeArrowPanel , STreeArrow)
		//SNew(STreeArrow)
	];
	ArrowChildren.Add(&ArrowSlot);

	const int32 NumSlots = InArgs.Slots.Num();
	for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
	{
		Children.Add(InArgs.Slots[SlotIndex]);
	}

	InitNode();
}

int32 SCanvasTree::RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
{
	for (int32 count = 0; count < Children.Num(); count++)
	{
		if (SlotWidget == Children.GetChildAt(count))
		{
			Children.RemoveAt(count);
			return count;
		}
	}

	return -1;
}

#pragma  optimize("", off)
int32 SCanvasTree::RemoveSlot(int32 NodeId)
{
	for (int32 count = 0; count < Children.Num(); count++)
	{
		if (NodeId == Children[count].m_NodeID)
		{
			Children.RemoveAt(count);
			return count;
		}
	}

	return -1;
}
#pragma  optimize("", on)

SCanvasTree::FSlot* SCanvasTree::GetSlot(const TSharedRef<SWidget>& SlotWidget)
{
	for (int32 count = 0; count < Children.Num(); count++)
	{
		if (SlotWidget == Children.GetChildAt(count))
			return &Children[count];
	}

	return nullptr;
}
SCanvasTree::FSlot* SCanvasTree::GetSlot(int32 NodeId)
{
	for (int32 count = 0; count < Children.Num(); count++)
	{
		if (NodeId == Children[count].m_NodeID)
			return &Children[count];
	}
	return nullptr;
}

void SCanvasTree::ClearChildren()
{
	Children.Empty();
}

void SCanvasTree::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	if (Children.Num() > 0)
	{
		for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
		{
			const SCanvasTree::FSlot& CurChild = Children[ChildIndex];
			//const FVector2D Size = CurChild.SizeAttr.Get();
			//19.11.04 使用TreeNode自身的size
			const FVector2D Size = CurChild.GetWidget()->GetDesiredSize();

			//Handle HAlignment
			FVector2D Offset(0.0f, 0.0f);

			switch (CurChild.HAlignment)
			{
			case HAlign_Center:
				Offset.X = -Size.X / 2.0f;
				break;
			case HAlign_Right:
				Offset.X = -Size.X;
				break;
			case HAlign_Fill:
			case HAlign_Left:
				break;
			}

			//handle VAlignment
			switch (CurChild.VAlignment)
			{
			case VAlign_Bottom:
				Offset.Y = -Size.Y;
				break;
			case VAlign_Center:
				Offset.Y = -Size.Y / 2.0f;
				break;
			case VAlign_Top:
			case VAlign_Fill:
				break;
			}

			// Add the information about this child to the output list (ArrangedChildren)
			ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(
				// The child widget being arranged
				CurChild.GetWidget(),
				// Child's local position (i.e. position within parent)
				CurChild.PositionAttr.Get() + Offset,
				// Child's size
				Size
			));
		}
	}
}

void SCanvasTree::ArrangeArrow(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	if (ArrowChildren.Num() > 1)
	{
		//先绘制一个背景

		const SCanvasTree::FSlot& BgChild = ArrowChildren[0];
		const FVector2D BgSize = M_SIZE;
		ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(
			// The child widget being arranged
			BgChild.GetWidget(),
			// Child's local position (i.e. position within parent)
			BgChild.PositionAttr.Get(),
			// Child's size
			BgSize
		));
		//再绘制一个箭头层
		const SCanvasTree::FSlot& ArrowChild = ArrowChildren[1];
		ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(
			// The child widget being arranged
			ArrowChild.GetWidget(),
			// Child's local position (i.e. position within parent)
			ArrowChild.PositionAttr.Get(),
			// Child's size
			BgSize
		));
	}
}


#pragma  optimize("", off)
int32 SCanvasTree::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	/************************************************************************/
	/*							绘制箭头等相关！		 				 */
	/************************************************************************/
	//获取一次笔刷设置一个边框 ，若没有背景图的绘制，就无法响应点击
	{
		/*const FSlateBrush* BorderBrush = BorderImg.Get();

		if (BorderBrush && BorderBrush->DrawAs != ESlateBrushDrawType::NoDrawType)
		{
			const ESlateDrawEffect DrawEffects = ESlateDrawEffect::DisabledEffect;
			//判断下是否反向，但是现阶段不考虑
			if (GSlateFlowDirection == EFlowDirection::RightToLeft)
			{
				const FGeometry FlippedGeometry = AllottedGeometry.MakeChild(FSlateRenderTransform(FScale2D(-1, 1)));
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId,
					FlippedGeometry.ToPaintGeometry(),
					BorderBrush,
					DrawEffects,
					BorderBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * FLinearColor::White
				);
			}
			else
			{
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId,
					AllottedGeometry.ToPaintGeometry(),
					BorderBrush,
					DrawEffects,
					BorderBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * FLinearColor::White//BorderBackgroundColor.Get().GetColor(InWidgetStyle)
				);
			}
			//尝试画一条线
			TArray<FVector2D> LinePoints = { FVector2D( 0, 0 ), FVector2D( 10,50),FVector2D( 50,90) };
			const FGeometry FlippedLineGeometry = AllottedGeometry.MakeChild(FSlateRenderTransform(FScale2D(1, 1)));
			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				LinePoints,
				ESlateDrawEffect::None,
				FLinearColor::White,
				true,
				5
			);
		}*/

		int32 ArrowLayerId = LayerId;

		FArrangedChildren ArrangedArrowChildren(EVisibility::Visible);
		this->ArrangeArrow(AllottedGeometry, ArrangedArrowChildren);

		const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

		const FPaintArgs NewArgs = Args.WithNewParent(this);

		for (int32 ChildIndex = 0; ChildIndex < ArrangedArrowChildren.Num(); ++ChildIndex)
		{
			FArrangedWidget& CurWidget = ArrangedArrowChildren[ChildIndex];

			if (!IsChildWidgetCulled(MyCullingRect, CurWidget))
			{
				const int32 CurWidgetsMaxLayerId = CurWidget.Widget->Paint(NewArgs, CurWidget.Geometry, MyCullingRect, OutDrawElements, ArrowLayerId + 1, InWidgetStyle, bForwardedEnabled);

				ArrowLayerId = FMath::Max(ArrowLayerId, CurWidgetsMaxLayerId);
			}
		}

		LayerId = ArrowLayerId;
	}

	/************************************************************************/
	/*							绘制箭头等相关！		 				 */
	/************************************************************************/

	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	this->ArrangeChildren(AllottedGeometry, ArrangedChildren);

	// Because we paint multiple children, we must track the maximum layer id that they produced in case one of our parents
	// wants to an overlay for all of its contents.
	int32 MaxLayerId = LayerId;

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

	const FPaintArgs NewArgs = Args.WithNewParent(this);

	for (int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ++ChildIndex)
	{
		FArrangedWidget& CurWidget = ArrangedChildren[ChildIndex];

		if (!IsChildWidgetCulled(MyCullingRect, CurWidget))
		{
			const int32 CurWidgetsMaxLayerId = CurWidget.Widget->Paint(NewArgs, CurWidget.Geometry, MyCullingRect, OutDrawElements, MaxLayerId + 1, InWidgetStyle, bForwardedEnabled);

			MaxLayerId = FMath::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}
#pragma  optimize("", on)

void SCanvasTree::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SPanel::OnFocusLost(InFocusEvent);

	//Release();
}

FVector2D SCanvasTree::ComputeDesiredSize(float) const
{
	// Canvas widgets have no desired size -- their size is always determined by their container
	return FVector2D::ZeroVector;
	//return FVector2D(500, 500);
}

FChildren* SCanvasTree::GetChildren()
{
	return &Children;
}

/////////////////////鼠标点击事件相关

bool SCanvasTree::SupportsKeyboardFocus() const
{
	return true;
}

bool SCanvasTree::IsInteractable() const
{
	return true;
}

//判断如果是右键，则再点击位置生成一个TreeNode
#pragma  optimize("", off)
FReply SCanvasTree::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();

	//暂时写的右键为添加Tree
	if (IsEnabled() && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		//计算一下生成位置，因为有一个相对位置的问题
		FVector2D CreatePos = MouseEvent.GetScreenSpacePosition();
		FVector2D absolutePos = MyGeometry.GetAbsolutePosition();
		FVector2D ClickSizeInTree = CreatePos - absolutePos;

		//生成在中心店
		ClickSizeInTree.X -= 50;
		ClickSizeInTree.Y -= 30;

		//this->AddSlot().Position(ClickSizeInTree)[
		//	SNew(STreeNode,this).ClickNodeCallBack(this, &SCanvasTree::ClickNodeCall).UpNodeCallBack(this, &SCanvasTree::UpNodeCall)
		//];
		CreateNewNode(ClickSizeInTree);

		Reply = FReply::Handled();
	}
	//左键为开始画线，此事件因为TreeNode的冒泡事件
	if (IsEnabled() && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && BStartDraw == true && BNodeMove == false)
	{
		//计算一下生成位置，因为有一个相对位置的问题
		FVector2D CreatePos = MouseEvent.GetScreenSpacePosition();
		FVector2D absolutePos = MyGeometry.GetAbsolutePosition();
		FVector2D ClickSizeInTree = CreatePos - absolutePos;

		mTreeArrowPanel->StartDrawArrow(ClickSizeInTree);

		Reply = FReply::Handled();
	}
	//测试用 鼠标中间按下，保存
	if (IsEnabled() && MouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton)
	{
		SaveAllNodes();
		Reply = FReply::Handled();
	}

	return Reply;
}
#pragma  optimize("", on)

//收到Tree的点击事件
void SCanvasTree::ClickNodeCall(FVector2D Pos, STreeNode* _CurNode)
{
	//mTreeArrowPanel->StartDrawArrow(Pos);
	//设置初始点击位置
	FSlot* tempSlot = GetSlot(_CurNode->M_NodeData->DataID);
	tempSlot->StartClickPos = tempSlot->PositionAttr.Get();

	BNodeMove = _CurNode->BNodeMove;
	BStartDraw = true;
	CurNode = _CurNode;
}

//当点击后鼠标移动！
FReply SCanvasTree::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (BStartDraw == true && BNodeMove == false)
	{
		FReply Reply = FReply::Unhandled();
		FVector2D CreatePos = MouseEvent.GetScreenSpacePosition();
		FVector2D absolutePos = MyGeometry.GetAbsolutePosition();
		FVector2D ClickSizeInTree = CreatePos - absolutePos;

		mTreeArrowPanel->MoveDrawArrow(ClickSizeInTree);

		return FReply::Handled();
	}
	/* 19.11.20 在移动时设置其位置 */
	if (BNodeMove == true)
	{
		FReply Reply = FReply::Unhandled();
		FVector2D CreatePos = MouseEvent.GetScreenSpacePosition();
		FVector2D absolutePos = MyGeometry.GetAbsolutePosition();
		FVector2D ClickSizeInTree = CreatePos - absolutePos;

		if(CurNode != nullptr)
			CurNode->SetNodeLinePos(CurNode->GetAbsoluteCenterLinePos() - absolutePos);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

//鼠标从节点上抬起时的回调
void SCanvasTree::UpNodeCall(FVector2D Pos, STreeNode* _LinkNode)
{
	LinkNode = _LinkNode;
}

//应该是由Node发起的结束绘制,鼠标抬起时，一定会清除当前选中节点
FReply SCanvasTree::OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	//左键抬起时结束本次绘制,判断下是否处于点中状态，判断下是否抬起点为当前节点
	if (IsEnabled() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (BStartDraw == true && BNodeMove == false && LinkNode != nullptr && LinkNode != CurNode && CheckNodeCanBeConnect())
		{
			BStartDraw = false;
			BNodeMove = true;
			FVector2D CreatePos = InMouseEvent.GetScreenSpacePosition();
			FVector2D absolutePos = InMyGeometry.GetAbsolutePosition();
			FVector2D ClickSizeInTree = CreatePos - absolutePos;

			//mTreeArrowPanel->EndDrawArrow(ClickSizeInTree , FVector2D::ZeroVector);
			int32 CurAID = mTreeArrowPanel->EndDrawArrow(CurNode->GetAbsoluteCenterLinePos() - absolutePos, LinkNode->GetAbsoluteCenterLinePos() - absolutePos 
				, CurNode->M_NodeData->DataID , LinkNode->M_NodeData->DataID);

			//设置两个节点的父子节点
			CurNode->SetChildNode(LinkNode);
			CurNode->SetNodeLinePos(CurNode->GetAbsoluteCenterLinePos() - absolutePos);
			CurNode->SetParentAID(CurAID);
			LinkNode->SetParentNode(CurNode);
			LinkNode->SetNodeLinePos(LinkNode->GetAbsoluteCenterLinePos() - absolutePos);
			LinkNode->SetChildAID(CurAID);
		}
		else //不满足条件则要清空当前绘制点
			mTreeArrowPanel->ClearCurDrawArrow();
	}
	
	ClearCurNode();
	BStartDraw = false;

	return FReply::Unhandled();
}

void SCanvasTree::ClearCurNode()
{
	CurNode = nullptr;
	LinkNode = nullptr;
}

bool SCanvasTree::CheckNodeCanBeConnect()
{
	bool result = true;

	//if (CurNode->GetChildNode() != nullptr)
	if (CurNode->M_NodeData->ChildID != -1)
		result = false;
	//if (LinkNode->GetParentNode() != nullptr)
	if (LinkNode->M_NodeData->ParentID != -1)
		result = false;

	return result;
}

/* 删除节点回调
*/
void SCanvasTree::DeleteNodeCall(STreeNode* _DelNode)
{
	//清除对应的父子关系以及对应父子关系上的
	if (_DelNode->M_NodeData->ParentID != -1)
	{
		STreeNode* Parent = NodeArray[_DelNode->M_NodeData->ParentID];
		//Parent->M_NodeData->ChildID = -1;
		Parent->ClearChildNode();
	}
	if (_DelNode->M_NodeData->ChildID != -1)
	{
		STreeNode* ChildNode = NodeArray[_DelNode->M_NodeData->ChildID];
		//Parent->M_NodeData->ParentID = -1;
		ChildNode->ClearParentNode();
	}
	
	//清除Arrow
	mTreeArrowPanel->ClearArrow(_DelNode->GetParentAID());
	mTreeArrowPanel->ClearArrow(_DelNode->GetChildAID());

	//NodeArray中删除
	NodeArray.Remove(_DelNode->M_NodeData->DataID);
	//直接把Slot删除
	RemoveSlot(_DelNode->M_NodeData->DataID);
}

/*
移动节点回调
*/
void SCanvasTree::MoveNodeCall(STreeNode* _MoveNode , FVector2D _MoveOffset)
{
	//在slot中设置位置，
	FSlot* tempSlot = GetSlot(_MoveNode->M_NodeData->DataID);
	UE_LOG(LogTemp, Warning, TEXT(" POSX : %f    +  OFFSETX :  %f") , tempSlot->StartClickPos.X , _MoveOffset.X);
	UE_LOG(LogTemp, Warning, TEXT(" POSY : %f    +  OFFSETY :  %f"), tempSlot->StartClickPos.Y, _MoveOffset.Y);
	if (tempSlot != nullptr)
		tempSlot->Position(tempSlot->StartClickPos + _MoveOffset);
	_MoveNode->SetNodePos(tempSlot->PositionAttr.Get());

	//设置箭头位置 19.11.20 继续开始
	/* 找到该节点作为父和子的两条线段，分别修改其线段的起始段和结束端 */
	mTreeArrowPanel->MoveArrowByNode(_MoveNode);

}

/******************************************
	数据相关
*/
void SCanvasTree::InitNode()
{
	/*NodeData* testData = new NodeData(1);
	testData->Pos = FVector2D(40, 40);
	testData->LinePos = FVector2D(40+50, 40+30);
	testData->ChildID = 2;

	NodeData* testData2 = new NodeData(2);
	testData2->Pos = FVector2D(140, 140);
	testData2->LinePos = FVector2D(140 + 50, 140 + 30);
	testData->ParentID = 1;
	
	NodeDataList = new TreeData();
	NodeDataList->DataList.Add(testData->DataID, testData);
	NodeDataList->DataList.Add(testData2->DataID, testData2);

	CreateNode();
	CreateArrow();

	FString test = "aaaaaaaaa";
	//TreeDataMgr::SaveAllData(test);
	TreeDataMgr::SaveAllData(NodeDataList);*/

	NodeDataList = TreeDataMgr::ReadAllData();

	CreateNode();
	//CreateArrow();
}

void SCanvasTree::CreateNode()
{
	for(TMap<int32, NodeData*>::TConstIterator iter(NodeDataList->DataList);iter ;++iter)
	{
		TSharedPtr<STreeNode> NewTreeNode;
		NodeData* tempData = iter->Value;
		this->AddSlot().Position(tempData->Pos).NodeID(iter->Value->DataID)[  //并且设置Slot的标识
			SAssignNew(NewTreeNode , STreeNode, this).ClickNodeCallBack(this, &SCanvasTree::ClickNodeCall).UpNodeCallBack(this, &SCanvasTree::UpNodeCall)
				.DeleteNodeCallBack(this, &SCanvasTree::DeleteNodeCall).MoveNodeCallBack(this, &SCanvasTree::MoveNodeCall)
		];
		
		//计算一下鼠标动态生成的Node的ID
		CalulateNextNodeID(iter->Value->DataID);

		NodeArray.Add(iter->Value->DataID, NewTreeNode.Get());
		NewTreeNode.Get()->SetNodeData(tempData);
	}

	CreateArrow();

	//为所有的节点设置父子关系
	for (TMap<int32, STreeNode*>::TIterator iter(NodeArray); iter; ++iter)
	{
		STreeNode* Node = iter->Value;
		//设置子节点
		if (Node->M_NodeData->ChildID != -1)
		{
			STreeNode* child = NodeArray[Node->M_NodeData->ChildID];
			if(child != nullptr)
				Node->SetChildNode(child);
		}
		//设置父节点
		if (Node->M_NodeData->ParentID != -1)
		{
			STreeNode* parent = NodeArray[Node->M_NodeData->ParentID];
			if (parent != nullptr)
				Node->SetParentNode(parent);
		}
	}
}

void SCanvasTree::CreateArrow()
{
	TMap< int32, TArray<FVector2D>> ArrowList;
	TMap<int32, TPair< int32, int32 >> NodeRelationList;

	//寻找每一个节点的子对象，生成一个Arrow的列表用来生成箭头
	for (TMap<int32, NodeData*>::TConstIterator iter(NodeDataList->DataList); iter; ++iter)
	{
		NodeData* tempData = iter->Value;
		if (tempData->ChildID != -1)
		{
			NodeData* ChildData = NodeDataList->DataList[tempData->ChildID];
			if (ChildData != nullptr)
			{
				TArray<FVector2D> TempArrow;
				TempArrow = { tempData->LinePos , ChildData->LinePos };

				//获取一下当前AID
				int32 AID = mTreeArrowPanel->GetNextArrID();

				ArrowList.Add(AID,TempArrow);
				NodeRelationList.Add(AID, TPair<int32, int32>(tempData->DataID , tempData->ChildID));
			}
		}
	}

	mTreeArrowPanel->InitArrowData(ArrowList, NodeRelationList);
}

//通用调用，创建一个子节点
void SCanvasTree::CreateNewNode( FVector2D Pos)
{
	TSharedPtr<STreeNode> NewTreeNode;

	int32 curId = GetNextNodeID();

	this->AddSlot().Position(Pos).NodeID(curId)[  //并且设置Slot的标识
		SAssignNew(NewTreeNode, STreeNode, this).ClickNodeCallBack(this, &SCanvasTree::ClickNodeCall).UpNodeCallBack(this, &SCanvasTree::UpNodeCall)
			.DeleteNodeCallBack(this, &SCanvasTree::DeleteNodeCall).MoveNodeCallBack(this, &SCanvasTree::MoveNodeCall)
	];

	NodeArray.Add(curId, NewTreeNode.Get());
	NodeData* temp_Data = new NodeData(curId);
	temp_Data->Pos = Pos;
	NewTreeNode.Get()->SetNodeData(temp_Data);
}

void SCanvasTree::CalulateNextNodeID(int32 C_ID)
{
	if (NextNodeID < C_ID)
		NextNodeID = C_ID;
}

int32 SCanvasTree::GetNextNodeID()
{
	NextNodeID++;
	return NextNodeID;
}

/*遍历保存所有的节点！
*/
void SCanvasTree::SaveAllNodes()
{
	//先清空所有的列表
	NodeDataList->DataList.Empty();

	for ( TMap<int32 , STreeNode*>::TConstIterator iter(NodeArray);iter ; ++iter)
	{
		STreeNode* tempData = iter->Value;
		NodeDataList->DataList.Add(tempData->M_NodeData->DataID, tempData->M_NodeData);
	}

	TreeDataMgr::SaveAllData(NodeDataList);
}

#undef   LOCTEXT_NAMESPACE