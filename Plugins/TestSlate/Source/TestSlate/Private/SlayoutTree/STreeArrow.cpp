#include "STreeArrow.h"

STreeArrow::STreeArrow()
{

}

void STreeArrow::Construct(const FArguments& InArgs)
{
	Size = InArgs._Size.Get();
}


FVector2D STreeArrow::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return Size.Get() * SCompoundWidget::ComputeDesiredSize(LayoutScaleMultiplier);
}

int32 STreeArrow::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	//TArray<FVector2D> LinePoints = { FVector2D(0, 0), FVector2D(10,50),FVector2D(50,90) };
	if (ArrowList.Num() > 0)
	{
		//for (int ArrowCount = 0; ArrowCount < ArrowList.Num(); ArrowCount++)
		for (TMap< int32, TArray<FVector2D>>::TConstIterator iter(ArrowList);iter;++iter)
		{
			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				//ArrowList[ArrowCount],
				iter->Value,
				ESlateDrawEffect::None,
				FLinearColor::White,
				true,
				4
			);
			//画小箭头相关
			FVector2D endPos = iter->Value[1];//ArrowList[ArrowCount][1];
			FVector2D startPos = iter->Value[0];// ArrowList[ArrowCount][0];
			FVector2D DrawPos = FVector2D(startPos.X + (endPos.X - startPos.X)/2, startPos.Y + (endPos.Y - startPos.Y)/2);
			FVector2D DrawPosTest = FVector2D(DrawPos.X - 10, DrawPos.Y - 10);

			const FSlateBrush* BrushResource = FTestSlateStyle::Get().GetBrush("UI.ArrowBg");
			const FGeometry FlippedGeometry = AllottedGeometry.MakeChild(
				FSlateRenderTransform(FScale2D(0.05f, 0.05f) , DrawPosTest),FVector2D(0,0)
			);
			FSlateDrawElement::MakeRotatedBox(
				OutDrawElements,
				LayerId,
				FlippedGeometry.ToPaintGeometry(),
				BrushResource,
				ESlateDrawEffect::None,
				//ArrowDegrees,
				CalculateArrowRadians(iter->Value[0], iter->Value[1]),
				TOptional<FVector2D>(),
				FSlateDrawElement::RelativeToElement,
				FLinearColor::White
				//FLinearColor::White
			);

		}
	}
	//再绘制当前正在进行的绘制
	if(CurArrow.Num() > 0)
	{
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			CurArrow,
			ESlateDrawEffect::None,
			FLinearColor::White,
			true,
			5
		);
	}
	
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

//鼠标事件暂时只向上冒泡！
FReply STreeArrow::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();

	return Reply;
}

FReply STreeArrow::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

FReply STreeArrow::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

void STreeArrow::StartDrawArrow(FVector2D startPos)
{
	CurArrow.Empty();

	FVector2D StartPos = startPos;
	CurArrow.Add(StartPos);
}

void STreeArrow::MoveDrawArrow(FVector2D endPos)
{
	if (CurArrow.Num() > 0 && CurArrow.Num() < 2)
		CurArrow.Add(endPos);
	else
		CurArrow[1] = endPos;
}

int32 STreeArrow::EndDrawArrow(FVector2D startPos, FVector2D endPos, int32 ParentId, int32 ChildId)
{
	/*if (CurArrow.Num() > 0 && CurArrow.Num() < 2)
		CurArrow.Add(endPos);
	else
		CurArrow[1] = endPos;*/

	CurArrow = { startPos, endPos };

	int32 Cur_AID = GetNextArrID();

	ArrowList.Add(Cur_AID,CurArrow);
	//还要添加父子的对应关系
	NodeRelationList.Add(Cur_AID, TPair<int32, int32>(ParentId, ChildId));

	CurArrow.Empty();

	return Cur_AID;
}

void STreeArrow::ClearCurDrawArrow()
{
	CurArrow.Empty();
}

float STreeArrow::CalculateArrowRadians( FVector2D startPos , FVector2D endPos) const
{
	//在线上面画一个小箭头！  计算一下ATAN 就是a/b 
	float atanVal = 0;
	float ArrowDegrees = 0;
	float LineA = (endPos.X - startPos.X);
	float LineB = (endPos.Y - startPos.Y);
	float LineC = UKismetMathLibrary::Distance2D(startPos, endPos);
	//如果是需要箭头向下是ATAN(A/B)！如果是需要箭头向上！则是ATAN(B/A)
	if (endPos.Y < startPos.Y) //此时为向上指
		ArrowDegrees = -UKismetMathLibrary::Atan(LineA / LineB) + FMath::DegreesToRadians(180);
	else if (endPos.Y > startPos.Y)
		ArrowDegrees = -UKismetMathLibrary::Atan(LineA / LineB);
	else if (endPos.X > startPos.X)
		ArrowDegrees = FMath::DegreesToRadians(90);
	else
		ArrowDegrees = FMath::DegreesToRadians(-90);

	return ArrowDegrees;
}


/*
	数据相关
*/
//打开该界面的时候，初始化线段数据
void STreeArrow::InitArrowData(TMap< int32, TArray<FVector2D>> _ArrowList , TMap<int32, TPair< int32, int32 >> _NodeRelationList)
{
	ArrowList = _ArrowList;
	NodeRelationList = _NodeRelationList;
}

void STreeArrow::CalulateNextArrID(int32 Cur_AID)
{
	if (NextArrID < Cur_AID)
		NextArrID = Cur_AID;
}

int32 STreeArrow::GetNextArrID()
{
	NextArrID++;
	return NextArrID;
}

void STreeArrow::ClearArrow(int32 _AID)
{
	if (ArrowList.Num() > 0 && ArrowList.Find(_AID) != nullptr && NodeRelationList.Num()>0 && NodeRelationList.Find(_AID) != nullptr)
	{
		ArrowList.Remove(_AID);
		NodeRelationList.Remove(_AID);
	}
}

/*  */
void STreeArrow::MoveArrowByNode(STreeNode* _Node)
{
	int32 ParentArrowId = -1;
	int32 ChildArrowId = -1;

	/* 循环查找对应的父与子线段，获取Node的链接点，重新设置 */
	for (TMap<int32, TPair< int32, int32 >>::TConstIterator iter(NodeRelationList); iter; ++iter)
	{
		TPair< int32, int32 > CurRelation = iter->Value;
		if (CurRelation.Key == _Node->M_NodeData->DataID)
		{
			ParentArrowId = iter->Key;
			if (ChildArrowId != -1)	/* 如果两个值都已经查到了，就直接break*/
				break;
		}
		if (CurRelation.Value == _Node->M_NodeData->DataID)
		{
			ChildArrowId = iter->Key;
			if (ParentArrowId != -1)	/* 如果两个值都已经查到了，就直接break*/
				break;
		}
	}

	/*重新设置作为父类的Arrow起始点*/
	if (ParentArrowId != -1)
	{
		//if(ArrowList.Num>0 && ArrowList[ParentArrowId].Num() > 0)
			ArrowList[ParentArrowId][0] = FVector2D(_Node->M_NodeData->LinePos);
	}
	/*重新设置作为子类的Arrow的结束点*/
	if (ChildArrowId != -1)
	{
		//if (ArrowList.Num > 0 && ArrowList[ParentArrowId].Num() > 0)
			ArrowList[ChildArrowId][1] = FVector2D( _Node->M_NodeData->LinePos);
	}
}