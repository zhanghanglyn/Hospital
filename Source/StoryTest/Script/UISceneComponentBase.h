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

	//通过传入的UI类型等初始化,例如TEXT("WidgetBlueprint'/Game/UI/widget/HpBar.HpBar_C'")
	UUISceneComponentBase(FString BluprintPath);

	//通过传入继承自UUserWidget的C++static类来加载模板，例如 UUIWidgetComponentBase::StaticClass()
	UUISceneComponentBase(UClass *userWidget);

//扩展 重写父类方法相关
public:


//成员变量
protected:
	//UI实体
	UUserWidget *m_UIWidget;

	//WidgetComponent
	UUIWidgetComponentBase *m_WidgetComponent;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "WidgetConstruct")
	UClass *userWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetConstruct")
	FString BluprintPath;
	//根Root
	//USceneCaptureComponent *m_RootComponent;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//通过传入一个UUSERWIDGET类的StaticClass 在构造函数中创建一个UI
	void createUIWidget(UClass *_userWidget);
	//通过使用蓝图路径，创建一个UI
	void createUIWidget(FString _bluePrintPath);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	//test测试代码
	UFUNCTION(BlueprintCallable)
	void TestPlay();
};
