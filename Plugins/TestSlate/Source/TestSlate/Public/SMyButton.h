#pragma once
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "TestSlate/Public/TestSlateStyle.h"
#include "Widgets/Text/STextBlock.h"

//м╪пнндвж
#include "EditorStyleSet.h"
#include "EditorFontGlyphs.h"

class  SMyButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(SMyButton) {}
	SLATE_ATTRIBUTE( FText, BtnIcon)
	SLATE_ATTRIBUTE(FText, BtnText)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

};