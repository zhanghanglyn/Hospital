/**
@ 类描述
@Author zhangh
@Email  1
@Date   2019-11-01 15:26:55

可在其中生成可以拖拽的树,与SCanvas类似
FSlot中会记录每一个子树的位置
可以在其中生成箭头

*/


#pragma once
#include "TestSlate.h"
#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Layout/Visibility.h"
#include "SlotBase.h"
#include "Widgets/SWidget.h"
#include "Layout/Children.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SPanel.h"
#include "SlayoutTree/STreeArrow.h"
#include "SlayoutTree/TreeNode.h"

class FArrangedChildren;
class FPaintArgs;
class FSlateWindowElementList;
class STreeArrow;
class STreeNode;

class  SCanvasTree : public SPanel
{
public:
	class FSlot : public TSlotBase<FSlot>
	{
	public:
		FSlot& Position(const TAttribute<FVector2D>& InPosition)
		{
			PositionAttr = InPosition;
			return *this;
		}

		FSlot& Size(const TAttribute<FVector2D>& InSize)
		{
			SizeAttr = InSize;
			return *this;
		}

		FSlot& HAlign(EHorizontalAlignment InHAlignment)
		{
			HAlignment = InHAlignment;
			return *this;
		}

		FSlot& VAlign(EVerticalAlignment InVAlignment)
		{
			VAlignment = InVAlignment;
			return *this;
		}

		FSlot& Expose(FSlot*& OutVarToInit)
		{
			OutVarToInit = this;
			return *this;
		}

		FSlot& NodeID(int32 _NodeID)
		{
			m_NodeID = _NodeID;
			return *this;
		}

		/** Position */
		TAttribute<FVector2D> PositionAttr;

		/** Size */
		TAttribute<FVector2D> SizeAttr;

		//该SLOT对应的Node的NodeID,删除用
		int32 m_NodeID;

		//移动用，点击开始时的初始位置
		FVector2D StartClickPos;

		/** Horizontal Alignment
		*  Given a top aligned slot, where '+' represents the
		*  anchor point defined by PositionAttr.
		*
		*   Left				Center				Right
			+ _ _ _ _            _ _ + _ _          _ _ _ _ +
			|		  |		   | 		   |	  |		    |
			| _ _ _ _ |        | _ _ _ _ _ |	  | _ _ _ _ |
		*
		*  Note: FILL is NOT supported.
		*/
		EHorizontalAlignment HAlignment;

		/** Vertical Alignment
		*   Given a left aligned slot, where '+' represents the
		*   anchor point defined by PositionAttr.
		*
		*   Top					Center			  Bottom
		*	+_ _ _ _ _          _ _ _ _ _		 _ _ _ _ _
		*	|		  |		   | 		 |		|		  |
		*	| _ _ _ _ |        +		 |		|		  |
		*					   | _ _ _ _ |		+ _ _ _ _ |
		*
		*  Note: FILL is NOT supported.
		*/
		EVerticalAlignment VAlignment;


		/** Default values for a slot. */
		FSlot()
			: TSlotBase<FSlot>()
			, PositionAttr(FVector2D::ZeroVector)
			, SizeAttr(FVector2D(1.0f, 1.0f))
			, HAlignment(HAlign_Left)
			, VAlignment(VAlign_Top)
			, m_NodeID(-1)
		{ }
	};

	SLATE_BEGIN_ARGS(SCanvasTree)
		:_BorderBrush(FCoreStyle::Get().GetBrush("Border"))
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
	}
	SLATE_SUPPORTS_SLOT(SCanvasTree::FSlot)  //会在自身注册一个Slots，所以在Construct中，可以获取Slots

	SLATE_ATTRIBUTE(const FSlateBrush* , BorderBrush)
	SLATE_END_ARGS()

	SCanvasTree();

	void Construct(const FArguments& InArgs, FVector2D Position , FVector2D Size);

	static FSlot& Slot()
	{
		return *(new FSlot());
	}

	FSlot& AddSlot()
	{
		SCanvasTree::FSlot& NewSlot = *(new FSlot());
		this->Children.Add(&NewSlot);
		return NewSlot;
	}

	//在ArrowChildren中添加Slot
	FSlot& AddArrowSlot()
	{
		SCanvasTree::FSlot& NewSlot = *(new FSlot());
		this->ArrowChildren.Add(&NewSlot);
		return NewSlot;
	}


	/**
	 * Removes a particular content slot.
	 *
	 * @param SlotWidget The widget in the slot to remove.
	 */
	int32 RemoveSlot(const TSharedRef<SWidget>& SlotWidget);
	int32 RemoveSlot(int32 NodeId);

	/*
	获取某个FSLot
	*/
	SCanvasTree::FSlot* GetSlot(const TSharedRef<SWidget>& SlotWidget);
	SCanvasTree::FSlot* GetSlot(int32 NodeId);

	/**
	 * Removes all slots from the panel.
	 */
	void ClearChildren();

public:

	void ArrangeArrow(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const;

	// SWidget overrides
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FChildren* GetChildren() override;
	virtual bool IsInteractable() const override;
	virtual bool SupportsKeyboardFocus() const override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

protected:
	// Begin SWidget overrides.
	virtual FVector2D ComputeDesiredSize(float) const override;
	// End SWidget overrides.

protected:
	/*
	TPanelChildren(SWidget* InOwner)
		: FChildren(InOwner)
		, bEmptying(false)
	{
	}  可见，初始化Children需要一个owner
	*/
	TPanelChildren<FSlot> Children;
	TPanelChildren<FSlot> ArrowChildren;

	//需要在外部传入保存一个自身的POSITION
	FVector2D M_Position;
	//自身SIZE
	FVector2D M_SIZE;

	//绘制边框的笔刷
	TAttribute<const FSlateBrush*> BorderImg;

	TSharedPtr<STreeArrow> mTreeArrowPanel;

	bool BStartDraw = false;

	bool BNodeMove = false;

//事件函数相关
public:
	//点击节点的回调
	void ClickNodeCall(FVector2D Pos , STreeNode* _CurNode);
	//当鼠标从一个节点移开的回调
	void UpNodeCall(FVector2D Pos, STreeNode* _LinkNode);
	//清除当前节点和离开节点
	void ClearCurNode();
	//判断当前松开点是否可以连接（暂时不支持选择节点之类的，一个节点只能有一个父类一个子类）
	bool CheckNodeCanBeConnect();
	//删除节点回调
	void DeleteNodeCall( STreeNode* _DelNode );
	//移动节点的回调
	void MoveNodeCall(STreeNode* _MoveNode , FVector2D _MoveOffset);

private:
	//节点Array
	TMap< int32, STreeNode*> NodeArray;
	//当前节点
	STreeNode* CurNode;
	//鼠标松开后的链接节点
	STreeNode* LinkNode;
	//数据List
	TreeData* NodeDataList;
	//下一个创建的Node的ID
	int32 NextNodeID = 0;

//数据相关	
public:
	void InitNode();
	//逐个创建子节点
	void CreateNode( );
	//根据所有数据创建线
	void CreateArrow();
	//通用创建一个子节点
	void CreateNewNode(FVector2D Pos);

protected:
	//更新下一个创建的Node的ID
	void CalulateNextNodeID(int32 C_ID);
	//获取下一个创建的NODE的ID
	int32 GetNextNodeID();
	//遍历所有节点并且将其保存
	void SaveAllNodes();
};