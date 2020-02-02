
#include "SWidgetTestA.h"

#define  LOCTEXT_NAMESPACE "SWidgetTestA"

void SWidgetTestA::Construct(const FArguments& InArgs)
{
	FString Button1Test = InArgs._TestAttribute1.Get();


	FText buttonText1 = FText::Format(
		LOCTEXT("WindowWidgetText", "just show it : {0}"),
		FText::FromString(TEXT("nothing, i just wanna to write"))
	);

	this->ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left).VAlign(VAlign_Top).Padding(10,5)
		[
			//SNew(SEventTest)
			//.OnClickTest(this,&SWidgetTestA::OnClickLogin)
			SNew(SMyButton).BtnIcon(FEditorFontGlyphs::Android)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left).VAlign(VAlign_Top).Padding(10, 5)
		[
			SNew(SMyButton).BtnIcon(FEditorFontGlyphs::Android)
		]
        /*SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString(Button1Test))
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString(FString("My Button 2")))
			.OnClicked(this, &SWidgetTestA::OnclickButton1 )
		]*/

	];
}

FReply SWidgetTestA::OnclickButton1()
{
	UE_LOG(LogTemp, Warning , TEXT("clicked button 1"))

	return FReply::Handled();
}

void SWidgetTestA::OnClickLogin(FString name, FString psw)
{
	UE_LOG(LogTemp, Warning, TEXT("clicked OnClickLogin !!"))
}

#undef   LOCTEXT_NAMESPACE