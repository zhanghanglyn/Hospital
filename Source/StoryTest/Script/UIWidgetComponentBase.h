// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Script/UserWidgetBase.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "UIWidgetComponentBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = "UserInterface" ,hidecategories = (Object, Activation, "Components|Activation", Sockets, Base, Lighting, LOD, Mesh), editinlinenew, meta = (BlueprintSpawnableComponent))
class STORYTEST_API UUIWidgetComponentBase : public UWidgetComponent
{
	GENERATED_BODY()

//扩展 重写父类方法相关
public:
	//设置UI并且返回USERWIDGET
	UUserWidget* SetWidgetClassReturnWidgetBase(TSubclassOf<UUserWidget> InWidgetClass);
	template<typename T>
	T* SetWidgetClassReturnWidgetBase(TSubclassOf<UUserWidget> InWidgetClass)
	{
		if (WidgetClass != InWidgetClass)
		{
			WidgetClass = InWidgetClass;

			if (HasBegunPlay())
			{
				if (WidgetClass)
				{
					T* NewWidget = CreateWidget<T>(GetWorld(), WidgetClass);
					SetWidget(Cast<UUserWidget>(NewWidget));
					return NewWidget;
				}
				else
				{
					SetWidget(nullptr);
				}
			}
		}

		return nullptr;
	}

	//扩展tick
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);


//静态方法相关
public:
	/**
	创建一个UIWIDGETCOMPONENT组件并且注册
	*param:outer	 :    父SceneComponent
	*param:bAttachTo :	  创建后是否Attach到父节点下
	*/
	static UUIWidgetComponentBase* CreateUIWidgetBase(USceneComponent *outer, bool _bAttachTo, bool _bToCamera);

//可在蓝图中设置的变量相关
public:
	//是否朝向摄像机
	UPROPERTY( EditAnywhere ,BlueprintReadWrite , Category = "GamePlayData")
	bool bToCamera;

//内部成员函数
protected:


//GamePlay函数相关
public:
	//通过传入一个UUSERWIDGET类的StaticClass 在构造函数中创建一个UI
	UUserWidget* SetUI(UClass *_userWidget);
	//通过使用蓝图路径，创建一个UI
	UUserWidget* SetUI(FString _bluePrintPath);
	//通过使用蓝图路径，创建一个UI,模板函数
	template< typename T1 >
	T1* SetUI(FString _bluePrintPath)
	{
		UClass* Temp_Widget = LoadClass<T1>(NULL, _bluePrintPath.GetCharArray().GetData());
		T1* tempWidget = SetWidgetClassReturnWidgetBase<T1>(Temp_Widget);

		return Cast<T1>(tempWidget);
	}

	//获取WIDGET
	UUserWidget* GetWidget();

	//设置相对位置
	void SetUIPosition(FVector _position );
	void SetUIRotation(FRotator _rotator);
};
