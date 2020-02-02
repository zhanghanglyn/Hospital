#include "..\Public\SMyButton.h"

#define  LOCTEXT_NAMESPACE "SMyButton"

void SMyButton::Construct(const FArguments & InArgs)
{
	SButton::Construct(
		SButton::FArguments()
		.ContentPadding(FMargin(50, 0))
		.Content()[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.16"))
				.Text(InArgs._BtnIcon)
			]
		]
		.ButtonStyle(
			&FTestSlateStyle::Get().GetWidgetStyle<FButtonStyle>("UI.Button1Style")
		)

	);
}

#undef LOCTEXT_NAMESPACE