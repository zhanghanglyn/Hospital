/**
@ 类描述
@Author zhangh
@Email  1
@Date   2019-10-31 14:57:34

自定义一个包含了背景图的SVerticalBoxPanel


*/


#pragma once
#include "TestSlate/Public/TestSlateStyle.h"
#include "SlateCore/Public/Widgets/Images/SImage.h"
#include "SlateCore/Public/Brushes/SlateImageBrush.h"
#include "SlateCore/Public/Widgets/SBoxPanel.h"

class  SVerticalBoxEx : public SBoxPanel
{
public:
	class FSlot : public SBoxPanel::FSlot
	{
	public:

		FSlot(): SBoxPanel::FSlot()
		{
		}

		FSlot& AutoHeight()
		{
			SizeParam = FAuto();
			return *this;
		}

		FSlot& MaxHeight( const TAttribute<float>& InMaxHeight )
		{
			MaxSize = InMaxHeight;
			return *this;
		}

		FSlot& FillHeight(const TAttribute< float >& StretchCoefficient)
		{
			SizeParam = FStretch(StretchCoefficient);
			return *this;
		}

		FSlot& Padding(float Uniform)
		{
			SlotPadding = FMargin(Uniform);
			return *this;
		}

		FSlot& Padding(float Horizontal, float Vertical)
		{
			SlotPadding = FMargin(Horizontal, Vertical);
			return *this;
		}

		FSlot& Padding(float Left, float Top, float Right, float Bottom)
		{
			SlotPadding = FMargin(Left, Top, Right, Bottom);
			return *this;
		}

		FSlot& Padding(const TAttribute<FMargin>::FGetter& InDelegate)
		{
			SlotPadding.Bind(InDelegate);
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

		FSlot& Padding(TAttribute<FMargin> InPadding)
		{
			SlotPadding = InPadding;
			return *this;
		}
		FSlot& operator[](TSharedRef<SWidget> InWidget)
		{
			SBoxPanel::FSlot::operator[](InWidget);
			return *this;
		}

		FSlot& Expose(FSlot*& OutVarToInit)
		{
			OutVarToInit = this;
			return *this;
		}

	};

	static FSlot& Slot()
	{
		return *(new FSlot());
	}


public:
	SLATE_BEGIN_ARGS(SVerticalBoxEx)
		: _ContentMargin(FMargin(0,0,0,0))
		, _BGSize( FVector2D::ZeroVector )
		, _BGImage(FCoreStyle::Get().GetDefaultBrush())
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
	}
	SLATE_ATTRIBUTE(FMargin , ContentMargin);
	SLATE_ATTRIBUTE(FVector2D, BGSize);
	SLATE_ATTRIBUTE(const FSlateBrush*, BGImage)
	SLATE_SUPPORTS_SLOT(SVerticalBoxEx::FSlot)
	SLATE_END_ARGS()

	FORCENOINLINE SVerticalBoxEx()
		: SBoxPanel(Orient_Vertical)
	{
		SetCanTick(false);
		bCanSupportFocus = false;
	}

	FSlot& AddSlot()
	{
		SVerticalBoxEx::FSlot& NewSlot = *new FSlot();
		this->Children.Add(&NewSlot);

		Invalidate(EInvalidateWidget::Layout);

		return NewSlot;
	}

	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;


	void Construct(const FArguments& InArgs);
private:

	FMargin m_ContentMargin;
	FVector2D m_BGSize;
};