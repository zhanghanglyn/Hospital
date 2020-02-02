#include "ScenarioConnectionDrawingPolicy.h"
#include "DrawElements.h"
#include "ScenarioNodeNormal.h"
#include "ConditionConversionNodeBase.h"

//其实只是设置了一下颜色以及Pin等参数并且通过ApplyHoverDeemphasis设置到成员函数中
void FScenarioConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params)
{
	Params.AssociatedPin2 = InputPin;
	Params.AssociatedPin1 = OutputPin;
	Params.WireThickness = 2;

	Params.WireColor = GetTransitionColor(HoveredPins.Contains(OutputPin));

	//颜色等参数获取了以后，应用一下
	if (HoveredPins.Num() > 0)
	{
		ApplyHoverDeemphasis(Params.AssociatedPin1, Params.AssociatedPin2, Params.WireThickness, Params.WireColor);
	}

}

//会在里面调用DrawSplineWithArrow
void FScenarioConnectionDrawingPolicy::Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries, FArrangedChildren& ArrangedNodes)
{
	FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes);
}

//将图形转换计算出开始点和结束点并且赋予重载函数 , 两个参数分别是pin的Geometry
void FScenarioConnectionDrawingPolicy::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params)
{
	// 取两个点的中间！
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// 在图形中找一个最靠近传入点的位置！
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	DrawSplineWithArrow(StartAnchorPoint, EndAnchorPoint, Params);
}


void FScenarioConnectionDrawingPolicy::DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint, const FConnectionParams& Params)
{
	DrawLineWithArrowImpl(StartPoint, EndPoint , Params);
}

void FScenarioConnectionDrawingPolicy::DrawPreviewConnector(const FGeometry & PinGeometry, const FVector2D & StartPoint, const FVector2D & EndPoint, UEdGraphPin * Pin)
{
	FConnectionParams Params;
	DetermineWiringStyle(Pin, nullptr, /*inout*/ Params);

	const FVector2D SeedPoint = EndPoint;
	const FVector2D AdjustedStartPoint = FGeometryHelper::FindClosestPointOnGeom(PinGeometry, SeedPoint);

	DrawSplineWithArrow(AdjustedStartPoint, EndPoint, Params);
}

FVector2D FScenarioConnectionDrawingPolicy::ComputeSplineTangent(const FVector2D & Start, const FVector2D & End) const
{
	const FVector2D Delta = End - Start;
	const FVector2D NormDelta = Delta.GetSafeNormal();

	return NormDelta;
}

//该函数作用是用来计算StartWidgetGeometry 与 EndWidgetGeometry 给之后的绘制函数使用，所以，在此我们根据自己定义的节点形状，来进行区分
void FScenarioConnectionDrawingPolicy::DetermineLinkGeometry(FArrangedChildren& ArrangedNodes,TSharedRef<SWidget>& OutputPinWidget,
	UEdGraphPin* OutputPin,UEdGraphPin* InputPin,
	/*out*/ FArrangedWidget*& StartWidgetGeometry,
	/*out*/ FArrangedWidget*& EndWidgetGeometry
)
{

	UScenarioNodeNormal* TestInputPin = Cast<UScenarioNodeNormal>(InputPin->GetOwningNode());
	UScenarioNodeNormal* TestOutPin = Cast<UScenarioNodeNormal>(OutputPin->GetOwningNode());

	//如果是绘制NormalNode , 因为没有Default的引脚，所以要在所有图形中去取引脚！
	//if (UScenarioNodeNormal* NormalNode = Cast<UScenarioNodeNormal>(OutputPin->GetOwningNode()))
	if(TestOutPin != nullptr && TestInputPin != nullptr )
	{
		UScenarioNodeNormal* NormalNode = Cast<UScenarioNodeNormal>(OutputPin->GetOwningNode());
		//去找所有图形中的SGraphNode并且看Node的UNodeObj
		UScenarioNodeNormal* InputNode = CastChecked<UScenarioNodeNormal>(InputPin->GetOwningNode());

		for (int32 NodeIndex = 0; NodeIndex < ArrangedNodes.Num(); ++NodeIndex) {
			FArrangedWidget& CurWidget = ArrangedNodes[NodeIndex];
			TSharedRef<SGraphNode> ChildNode = StaticCastSharedRef<SGraphNode>(CurWidget.Widget);
			if (ChildNode->GetNodeObj() == NormalNode) {
				StartWidgetGeometry = &(ArrangedNodes[NodeIndex]);
			}
			else if (ChildNode->GetNodeObj() == InputNode)
			{
				EndWidgetGeometry = &(ArrangedNodes[NodeIndex]);
			}
		}
	}
	//如果是绘制连接中间节点
	else if (UConditionConversionNodeBase* TransNode = Cast<UConditionConversionNodeBase>(InputPin->GetOwningNode()))
	{
		UScenarioNodeNormal* PrevState = TransNode->GetFromNode();
		UScenarioNodeNormal* NextState = TransNode->GetTargetNode();
		if ((PrevState != NULL) && (NextState != NULL)) {
			for (int32 NodeIndex = 0; NodeIndex < ArrangedNodes.Num(); ++NodeIndex) {
				FArrangedWidget& CurWidget = ArrangedNodes[NodeIndex];
				TSharedRef<SGraphNode> ChildNode = StaticCastSharedRef<SGraphNode>(CurWidget.Widget);
				if (ChildNode->GetNodeObj() == PrevState) {
					StartWidgetGeometry = &(ArrangedNodes[NodeIndex]);
				}
				if (ChildNode->GetNodeObj() == NextState) {
					EndWidgetGeometry = &(ArrangedNodes[NodeIndex]);
				}
			}
		}
	}
	else{
		StartWidgetGeometry = PinGeometries->Find(OutputPinWidget);
		//TSharedRef<SGraphPin> pTargetWidget = PinToPinWidgetMap.Find(InputPin);
		if (PinToPinWidgetMap.Find(InputPin))  //PinToPinWidgetMap是一个链接关系
		{
			TSharedRef<SGraphPin> InputWidget = (PinToPinWidgetMap.Find(InputPin)->ToSharedRef());
			EndWidgetGeometry = PinGeometries->Find(InputWidget);
		}
	}
}

void FScenarioConnectionDrawingPolicy::DrawLineWithArrowImpl(const FVector2D & StartAnchorPoint, const FVector2D & EndAnchorPoint, const FConnectionParams & Params)
{
	//@TODO: Should this be scaled by zoom factor?
	const float LineSeparationAmount = 4.5f;

	const FVector2D DeltaPos = EndAnchorPoint - StartAnchorPoint;
	const FVector2D UnitDelta = DeltaPos.GetSafeNormal();
	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	// Come up with the final start/end points
	const FVector2D DirectionBias = Normal * LineSeparationAmount;
	const FVector2D LengthBias = ArrowRadius.X * UnitDelta;
	const FVector2D StartPoint = StartAnchorPoint + DirectionBias + LengthBias;
	const FVector2D EndPoint = EndAnchorPoint + DirectionBias - LengthBias;

	// Draw a line/spline
	DrawConnection(WireLayerID, StartPoint, EndPoint, Params);

	// Draw the arrow
	const FVector2D ArrowDrawPos = EndPoint - ArrowRadius;
	const float AngleInRadians = FMath::Atan2(DeltaPos.Y, DeltaPos.X);

	FSlateDrawElement::MakeRotatedBox(
		DrawElementsList,
		ArrowLayerID,
		FPaintGeometry(ArrowDrawPos, ArrowImage->ImageSize * ZoomFactor, ZoomFactor),
		ArrowImage,
		ESlateDrawEffect::None,
		AngleInRadians,
		TOptional<FVector2D>(),
		FSlateDrawElement::RelativeToElement,
		Params.WireColor
	);
}


//获取自定义的Hover颜色以及普通的颜色
FLinearColor FScenarioConnectionDrawingPolicy::GetTransitionColor(bool bIsHovered)
{
	const FLinearColor HoverColor(0.724f, 0.256f, 0.0f, 1.0f);
	const FLinearColor BaseColor(0.9f, 0.5f, 0.6f, 1.0f);

	return bIsHovered ? HoverColor : BaseColor;
}