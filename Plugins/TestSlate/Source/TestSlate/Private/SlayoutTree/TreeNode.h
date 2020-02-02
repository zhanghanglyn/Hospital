/**
@ 类描述
@Author ZHANGH
@Email  @
@Date   2019-11-03 22:13:03

@实现一个树节点，该节点响应鼠标点击事件，并且有一个链表结构来表明自己的父节点和子节点，
@添加一个背景图片
@添加一个中间文字

*/


#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Layout/Margin.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/CoreStyle.h"
#include "TestSlate/Private/FileUtil/NodeDataUtil.h"
#include "SlayoutTree/SLayoutTree.h"
#include "TestSlate/Private/FileUtil/NodeDataUtil.h"

class SCanvasTree;

class  STreeNode : public SCompoundWidget
{
	DECLARE_DELEGATE_TwoParams( ClickNodeCall , FVector2D , STreeNode*)
	DECLARE_DELEGATE_TwoParams(UpNodeCall, FVector2D, STreeNode*)
	DECLARE_DELEGATE_OneParam(DeleteNodeCall, STreeNode*)
	DECLARE_DELEGATE_TwoParams(MoveNodeCall, STreeNode* , FVector2D)

public:
	enum ClickType
	{
		NONE = 0,
		CLICK_LINE = 1,
		CLICK_MOVE = 2,
	};

	struct A_Rect
	{
		float top;
		float left;
		float right;
		float bottom;
	};

public:
	SLATE_BEGIN_ARGS(STreeNode)
		:_ABgImageBrush( FTestSlateStyle::Get().GetBrush("UI.TreeNode"))
		, _ABorderBackgroundColor(FLinearColor::White)
		, _AbFlipForRightToLeftFlowDirection(false)
		, _bIsFocusable(true)
	{}
	SLATE_DEFAULT_SLOT(FArguments, Content)  //相当于绑定一个插槽叫做SLATE？

	SLATE_ATTRIBUTE( const FSlateBrush* , ABgImageBrush )
	SLATE_ATTRIBUTE(FSlateColor, ABorderBackgroundColor)
	SLATE_ATTRIBUTE(bool , AbFlipForRightToLeftFlowDirection)
	SLATE_ATTRIBUTE(bool, bIsFocusable)

	//绑定事件
	SLATE_EVENT(ClickNodeCall , ClickNodeCallBack)
	SLATE_EVENT(UpNodeCall , UpNodeCallBack)
	SLATE_EVENT(DeleteNodeCall, DeleteNodeCallBack)
	SLATE_EVENT(MoveNodeCall , MoveNodeCallBack)

	SLATE_END_ARGS()

	STreeNode();

	void Construct(const FArguments& InArgs , SCanvasTree* Tree);

	virtual void SetContent(TSharedRef< SWidget > InContent);
	//获取内容
	const TSharedRef< SWidget >& GetContent() const;
	//清除内容
	void ClearContent();
	//设置背景样式
	void SetBgImage(const TAttribute<const FSlateBrush*>& InBorderImage);
	/** Sets the color and opacity of the background image of this border. */
	void SetBorderBackgroundColor(const TAttribute<FSlateColor>& InColorAndOpacity);
	//设置大小 默认写死了 不设置算了
	//void SetDesiredSizeScale(const TAttribute<FVector2D>& InDesiredSizeScale);

public:
	// SWidget overrides
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool SupportsKeyboardFocus() const override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	//virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	//virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	//virtual void OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;*/
	virtual bool IsInteractable() const override;
	//virtual bool ComputeVolatility() const override;

protected:
	// Begin SWidget overrides.
	//计算设计大小
	virtual FVector2D ComputeDesiredSize(float) const override;
	// End SWidget overrides.

protected:

	TAttribute<const FSlateBrush*> BgImageBrush;
	//是否可以成为焦点
	bool bIsFocusable;

	TAttribute<FSlateColor> BorderBackgroundColor;
	/** Flips the image if the localization's flow direction is RightToLeft */
	bool bFlipForRightToLeftFlowDirection;
	//设计尺寸大小
	TAttribute<FVector2D> DesiredSizeScale;

	//记录一个开始点击的位置
	FVector2D PressedScreenSpacePosition;

//事件委托
protected:
	ClickNodeCall ClickNodeCallDelegate;
	UpNodeCall UpNodeCallDelegate;
	DeleteNodeCall DeleteNodeCallDelegate;
	MoveNodeCall MoveNodeCallDelegate;

//点击处理函数
protected:
	void PressFunction(FVector2D AbsolutePos);
	//判断鼠标点击区域是位于拖动区域还是画线区域
	ClickType CheckClickPosType( FVector2D ClickPos );

private:
	//Tree的指针
	SCanvasTree* CanvasTree;
	//父Node
	STreeNode* ParentNode;
	//子Node
	STreeNode* ChildNode;
	//自身的中心点
	FVector2D CenterPosition;
	//连线点
	FVector2D LinePos;
	//鼠标点击位置
	FVector2D MouseClickPos;

//外部使用逻辑函数
public:
	//设置自身连接点
	void SetAbsoluteCenterLinePos(FVector2D NodeStartPos);
	//获取自身连线点
	FVector2D GetAbsoluteCenterLinePos();
	//设置父节点
	void SetParentNode( STreeNode* _ParentNode);
	//设置子节点
	void SetChildNode(STreeNode* _ChildNode);
	//获取父节点
	STreeNode* GetParentNode();
	//获取子节点
	STreeNode* GetChildNode();
	//设置当前线段AIDD
	void SetChildAID(int32 _AID);
	int32 GetChildAID();
	void SetParentAID(int32 _AID);
	int32 GetParentAID();

	//清除父Node
	void ClearParentNode();

	//清除子Node
	void ClearChildNode();

	//设置自身Node数据
	void SetNodeData( NodeData* _NodeData);
	//设置自身LinePos
	void SetNodeLinePos(FVector2D _LinePos);
	//设置移动后自身位置
	void SetNodePos(FVector2D _NodePos);

//数据保存等相关
public:
	NodeData* M_NodeData;	//自身的Node数据
	int32 LineRange = 10;
	A_Rect* MoveRect;			//点击移动区域RECT

	//是否开始移动
	bool BNodeMove = false;
};