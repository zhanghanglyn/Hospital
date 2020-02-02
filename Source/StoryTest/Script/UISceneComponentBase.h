// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "UIWidgetComponentBase.h"
#include "UISceneComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STORYTEST_API UUISceneComponentBase : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUISceneComponentBase();

	//ͨ�������UI���͵ȳ�ʼ��,����TEXT("WidgetBlueprint'/Game/UI/widget/HpBar.HpBar_C'")
	UUISceneComponentBase(FString BluprintPath);

	//ͨ������̳���UUserWidget��C++static��������ģ�壬���� UUIWidgetComponentBase::StaticClass()
	UUISceneComponentBase(UClass *userWidget);

//��չ ��д���෽�����
public:


//��Ա����
protected:
	//UIʵ��
	UUserWidget *m_UIWidget;

	//WidgetComponent
	UUIWidgetComponentBase *m_WidgetComponent;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "WidgetConstruct")
	UClass *userWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetConstruct")
	FString BluprintPath;
	//��Root
	//USceneCaptureComponent *m_RootComponent;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//ͨ������һ��UUSERWIDGET���StaticClass �ڹ��캯���д���һ��UI
	void createUIWidget(UClass *_userWidget);
	//ͨ��ʹ����ͼ·��������һ��UI
	void createUIWidget(FString _bluePrintPath);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	//test���Դ���
	UFUNCTION(BlueprintCallable)
	void TestPlay();
};
