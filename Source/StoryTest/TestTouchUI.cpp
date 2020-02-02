// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTouchUI.h"

UTestTouchUI::UTestTouchUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTestTouchUI::NativeConstruct()
{
	Super::NativeConstruct();

	RootPanel = Cast<UCanvasPanel>(GetRootWidget());
	UWidget *tempa = RootPanel->GetChildAt(0);

	
	UCanvasPanel *tempb = Cast<UCanvasPanel>(RootPanel->GetChildAt(1));
	if (tempb != nullptr)
	{
		BF_Anima = Cast<UImage>(tempb->GetChildAt(0));
	}

	UWidget *tempC = GetWidgetFromName(FName(TEXT("BF")));

	UWidget *tempD = GetWidgetFromName(FName(TEXT("BF_1")));

	BF_Image = Cast<UImage>(GetWidgetFromName(FName(TEXT("BF_1"))));


	//BF_Anima = Cast<UImage>(GetWidgetFromName(FName(TEXT("BF"))));

}

//定时更新动画
void UTestTouchUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTestTouchUI::GetAllCtl()
{
	
}

void UTestTouchUI::SetPlay()
{
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager(); 

	if (BF_Image != nullptr)
	{
		UTexture2D *ChangeTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/StoryTest/Source/UI/3.3"));
		/*ChangeTexture->ConditionalBeginDestroy();
		ChangeTexture = NULL;
		GetWorld()->ForceGarbageCollection(true);*/

		//ChangeTexture = AssetLoader.LoadSynchronous<UTexture2D>(FSoftObjectPath("/Game/StoryTest/Source/UI/3.3"));
		if (ChangeTexture != nullptr)
			BF_Image->SetBrushFromTexture(ChangeTexture,false);
		//BF_Image->SetBrushFromMaterial();

	}

	//设置动画
	if (BF_Anima != nullptr)
	{
		//static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/StoryTest/Material/Material_BF_Inst.Material_BF_Inst'"));
		//UMaterial *tempMaterialC = Cast<UMaterial>(Material.Object);
		//if (tempMaterialC != nullptr)
			//UE_LOG(LogTemp, Warning, TEXT("tempMaterialC  !!!!!!!!!"));

		UMaterial *ChangeMaterial = AssetLoader.LoadSynchronous<UMaterial>(FSoftObjectPath("Material'/Game/StoryTest/Material/Material_BF.Material_BF'"));
		if (ChangeMaterial != nullptr)
			UE_LOG(LogTemp, Warning, TEXT("ChangeMaterial!!!!!!!!!"));
		//UMaterial *ChangeMaterial2 = LoadObject<UMaterial>(NULL, TEXT("Material'/Game/StoryTest/Material/Material_BF.Material_BF'"));
		//if (ChangeMaterial2 != nullptr)
			//UE_LOG(LogTemp, Warning, TEXT("ChangeMaterial2!!!!!!!!!!!!"));

		//UMaterial *ttt = nullptr;
		//UMaterialInstanceDynamic::Create(ChangeMaterial2, ttt);
		BF_Anima->SetBrushFromMaterial(ChangeMaterial);
	}
}