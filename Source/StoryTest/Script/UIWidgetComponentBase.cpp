// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidgetComponentBase.h"

/************************************************************************/
/*                         静态方法相关                                 */
/************************************************************************/
UUIWidgetComponentBase* UUIWidgetComponentBase::CreateUIWidgetBase(USceneComponent *outer, bool _bAttachTo = false, bool _bToCamera = true)
{

	UUIWidgetComponentBase *temp_WidgetComponent = NewObject<UUIWidgetComponentBase>(outer, UUIWidgetComponentBase::StaticClass());
	if (temp_WidgetComponent != nullptr)
	{
		temp_WidgetComponent->RegisterComponent();
		if (_bAttachTo == true)
			temp_WidgetComponent->AttachToComponent(outer, FAttachmentTransformRules::KeepRelativeTransform);

		temp_WidgetComponent->bToCamera = _bToCamera;

		return temp_WidgetComponent;
	}

	return nullptr;
}

/************************************************************************/
/*                       静态方法相关END                                */
/************************************************************************/

/************************************************************************/
/*                          重写扩展父类方法相关                        */
/************************************************************************/
UUserWidget * UUIWidgetComponentBase::SetWidgetClassReturnWidgetBase(TSubclassOf<UUserWidget> InWidgetClass)
{
	if (WidgetClass != InWidgetClass)
	{
		WidgetClass = InWidgetClass;

		if (HasBegunPlay())
		{
			if (WidgetClass)
			{
				UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
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

void UUIWidgetComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HasBegunPlay())
	{
		////////自动朝向
		if (Space != EWidgetSpace::Screen && bToCamera == true)
		{
			FRotator componentRotator = GetComponentRotation();
			FRotator cameraRotator = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation();

			this->SetWorldRotation(FRotator(-cameraRotator.Pitch, cameraRotator.Yaw + 180, cameraRotator.Roll + 180));
		}
	}
}

/************************************************************************/
/*                       重写扩展父类方法相关END                        */
/************************************************************************/

//通过传入一个UUSERWIDGET类的StaticClass 在构造函数中创建一个UI
UUserWidget* UUIWidgetComponentBase::SetUI(UClass *_userWidget)
{
	UUserWidget* temp_widget = this->SetWidgetClassReturnWidgetBase(_userWidget);

	//UUserWidgetBase *temp_result = Cast<UUserWidgetBase>(temp_widget);

	return temp_widget;
}

//通过使用蓝图路径，创建一个UI
UUserWidget* UUIWidgetComponentBase::SetUI(FString _bluePrintPath)
{
	UClass* Tmpe_Widget = LoadClass<UUserWidgetBase>(NULL, _bluePrintPath.GetCharArray().GetData());
	UUserWidget* temp_widget = this->SetWidgetClassReturnWidgetBase(Tmpe_Widget);

	//UUserWidgetBase *temp_result = Cast<UUserWidgetBase>(temp_widget);

	return temp_widget;
}

UUserWidget * UUIWidgetComponentBase::GetWidget()
{
	if (Widget != nullptr)
		return Widget;
	return nullptr;
}

void UUIWidgetComponentBase::SetUIPosition(FVector _position)
{
	this->SetRelativeLocation(_position);
}

void UUIWidgetComponentBase::SetUIRotation(FRotator _rotator)
{
	this->SetRelativeRotation(_rotator);
}
