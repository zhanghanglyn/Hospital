/**
@ 类描述
@Author zhangh
@Email  1
@Date   2019-11-05 15:49:31

在SCanvasTree中专门用来显示箭头的层，基本不用继承复杂功能（美好愿望）
根据传入的数据在对应位置生成箭头线
大小与父SCanvasTree一致
*/


#pragma once
#include "TestSlate/Public/TestSlate.h"
#include "TestSlate/Private/SlayoutTree/TreeNode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Math/UnrealMath.h"

class STreeNode;

class  STreeArrow : public SBorder
{
public:
	SLATE_BEGIN_ARGS(STreeArrow)
		:_Size(FVector2D::ZeroVector)
	{}
	SLATE_ATTRIBUTE(FVector2D , Size)
	SLATE_END_ARGS()

	STreeArrow();

	void Construct(const FArguments& InArgs);


public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

protected:
	// Begin SWidget overrides.
	virtual FVector2D ComputeDesiredSize(float) const override;
	// End SWidget overrides.

private:
	//与父类大小一致的尺寸
	TAttribute<FVector2D> Size;

	//线段的数据结构 TMap中的Key对应另一个TMAP，存储对应的子/父节点ID
	TMap< int32 ,TArray<FVector2D>> ArrowList;
	//TPair中第一项为父亲（起始Arrow)第二项为子类ID（终点ARRow）
	TMap<int32, TPair< int32, int32 >> NodeRelationList;

	//当前画线的数据,永远只有两个，一个起点，一个结束点
	TArray<FVector2D> CurArrow;

//箭头数据相关方法
public:
	//打开该界面的时候，初始化线段数据
	void InitArrowData(TMap< int32, TArray<FVector2D>> _ArrowList , TMap<int32, TPair< int32, int32 >> _NodeRelationList);
	//j计算箭头朝向
	float CalculateArrowRadians(FVector2D startPos, FVector2D endPos) const;

	//开始画线，确定起点位置
	void StartDrawArrow( FVector2D startPos );
	//一边移动一边进行划线
	void MoveDrawArrow(FVector2D endPos);
	//鼠标抬起时结束画线
	int32 EndDrawArrow(FVector2D startPos, FVector2D endPos , int32 ParentId, int32 ChildId);

	//清空当前绘制线条
	void ClearCurDrawArrow();

	//更新下一个创建的Arrow的ID
	void CalulateNextArrID(int32 _AID);
	//获取下一个创建的Arrow的ID
	int32 GetNextArrID();

	//根据AID清除一条线
	void ClearArrow(int32 _AID);

	/* 19.11.20 重新设置线段当一个节点移动时 */
	void MoveArrowByNode(STreeNode* _Node);

//数据相关
protected:
	int32 NextArrID = 0;
};