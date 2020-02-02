// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestSlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FTestSlateStyle::StyleInstance = NULL;

void FTestSlateStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FTestSlateStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FTestSlateStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("TestSlateStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon72x86(72.0f, 86.0f);

TSharedRef< FSlateStyleSet > FTestSlateStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("TestSlateStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("TestSlate")->GetBaseDir() / TEXT("Resources"));

	Style->Set("TestSlate.OpenPluginWindow", new IMAGE_BRUSH(TEXT("qipao"), Icon40x40));

	//定义一个上下左右的margin,相当于九宫格缩放的边界值
	//FMargin Button1Margin(34.0f/158.0f, 19.0f/59.0f, 34.0f / 158.0f, 19.0f / 59.0f);
	FMargin Button1Margin(0, 0, 0, 0);

	//设置一个按钮样式
	Style->Set("UI.Button1Style",
		FButtonStyle().SetNormal(
			FSlateBoxBrush(Style->RootToContentDir(TEXT("btn_normal"), TEXT(".png")), Button1Margin)
		).SetPressed(
			FSlateBoxBrush(Style->RootToContentDir(TEXT("btn_down"), TEXT(".png")), Button1Margin)
		).SetHovered(
			FSlateBoxBrush(Style->RootToContentDir(TEXT("btn_hold"), TEXT(".png")), Button1Margin)
		)
	);

	const FVector2D EditBoxBg(200, 152);

	//设置一个输入框样式
	Style->Set("UI.EditBoxStyle",
		FEditableTextBoxStyle().SetBackgroundImageNormal(
			FSlateImageBrush(Style->RootToContentDir(TEXT("bg1"),TEXT(".png")) , EditBoxBg)
		).SetBackgroundImageFocused(
			FSlateImageBrush(Style->RootToContentDir(TEXT("bg1"), TEXT(".png")), EditBoxBg)
		).SetBackgroundImageHovered(
			FSlateImageBrush(Style->RootToContentDir(TEXT("bg1"), TEXT(".png")), EditBoxBg)
		).SetFont( 
			FSlateFontInfo(Style->RootToContentDir(TEXT("FZYanSJW"), TEXT(".ttf")),32 )
		)
	);

	//设置一个图片样式
	const FVector2D imageBg(299, 446);
	Style->Set("UI.IMAGEtest1",
		new FSlateImageBrush(Style->RootToContentDir(TEXT("peoxiao"), TEXT(".png")), imageBg)
	);

	//设置一个背景图片样式
	const FVector2D VerticalBoxExBg(200, 400);
	Style->Set("UI.VerticalBoxExBg",
		new FSlateImageBrush(Style->RootToContentDir(TEXT("SverticalBoxBg"), TEXT(".png")), VerticalBoxExBg)
	);

	//创建一个箭头图片样式
	Style->Set("UI.ArrowBg",
		new FSlateImageBrush(Style->RootToContentDir(TEXT("Arrow"), TEXT(".png")), FVector2D(40, 40))
	);

	//创建一个树的背景样式TreeBg
	Style->Set("UI.TreeBg",
		new FSlateImageBrush(Style->RootToContentDir(TEXT("TreeBg"), TEXT(".png")), FVector2D(100, 100))
	);

	//创建一个树节点的背景
	Style->Set("UI.TreeNode",
		new FSlateImageBrush(Style->RootToContentDir(TEXT("NodeBg"), TEXT(".png")), FVector2D(100, 100))
	);

			
	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FTestSlateStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FTestSlateStyle::Get()
{
	return *StyleInstance;
}
