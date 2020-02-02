/**
@ 类描述
@Author zhangh
@Email  1
@Date   2019-10-28 16:49:54

1
*/
#pragma once
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "TestSlate/Public/TestSlateStyle.h"

class  SAutoLayout : public SBoxPanel
{
public:
	class FSlot : public SBoxPanel::FSlot
	{
	public:
		FSlot() : SBoxPanel::FSlot()
		{

		}

		FSlot& AutoHeight()
		{
			SizeParam = FAuto();
			return *this;
		}

		FSlot& MaxHeight(const TAttribute< float >& InMaxHeight)
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
	SLATE_BEGIN_ARGS(SAutoLayout){}
	SLATE_SUPPORTS_SLOT(SAutoLayout::FSlot)
	SLATE_END_ARGS()

	FSlot& AddSlot()
	{
		SAutoLayout::FSlot &NewSlot = *new SAutoLayout::FSlot();
		this->Children.Add(&NewSlot);

		Invalidate(EInvalidateWidget::Layout);

		return NewSlot;
	}

	void Construct(const FArguments& InArgs);
private:

};