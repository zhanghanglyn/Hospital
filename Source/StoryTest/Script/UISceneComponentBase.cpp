// Fill out your copyright notice in the Description page of Project Settings.


#include "UISceneComponentBase.h"

// Sets default values for this component's properties
UUISceneComponentBase::UUISceneComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//if ( !BluprintPath.IsEmpty())
	//	createUIWidget(BluprintPath);
	//else if(userWidget != NULL)
	//	createUIWidget(userWidget);
	// ...
}

//通过传入继承自UUserWidget的C++static类来加载模板，例如 UUIWidgetComponentBase::StaticClass()
UUISceneComponentBase::UUISceneComponentBase(UClass *userWidget)
{

	PrimaryComponentTick.bCanEverTick = true;

	createUIWidget(userWidget);
}

//通过传入的UI类型等初始化,例如TEXT("WidgetBlueprint'/Game/UI/widget/HpBar.HpBar_C'")
UUISceneComponentBase::UUISceneComponentBase(FString BluprintPath)
{

	PrimaryComponentTick.bCanEverTick = true;

	createUIWidget(BluprintPath);
}

// Called when the game starts
void UUISceneComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//////////////////////////////////////////////////////////////////////////////
////////////////////	 重写扩展父类方法相关	  ////////////////////////////
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
////////////////////	 重写扩展父类方法相关END	  ////////////////////////
/////////////////////////////////////////////////////////////////////////////




// Called every frame
void UUISceneComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//通过传入一个UUSERWIDGET类的StaticClass 在构造函数中创建一个UI
void UUISceneComponentBase::createUIWidget(UClass *_userWidget)
{
	//初始化widgetComponent
	if (m_WidgetComponent == NULL)
	{
		m_WidgetComponent = NewObject<UUIWidgetComponentBase>(GetTransientPackage(), UUIWidgetComponentBase::StaticClass());
		if (m_WidgetComponent != NULL)
		{
			m_WidgetComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			m_UIWidget = m_WidgetComponent->SetWidgetClassReturnWidgetBase(_userWidget);
			m_WidgetComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
			
	}
}

//通过使用蓝图路径，创建一个UI
void UUISceneComponentBase::createUIWidget(FString _bluePrintPath)
{
	//初始化widgetComponent
	if (m_WidgetComponent == NULL)
	{
		//CreateDefaultSubobject只能在构造函数中创建？？？
		m_WidgetComponent = NewObject<UUIWidgetComponentBase>(GetTransientPackage(), UUIWidgetComponentBase::StaticClass());
		//m_WidgetComponent = CreateDefaultSubobject<UUIWidgetComponentBase>(TEXT("UUIWidgetComponentBase"));
		if (m_WidgetComponent != NULL)
		{
			UClass* Widget = LoadClass<UUserWidget>(NULL, _bluePrintPath.GetCharArray().GetData());
			m_UIWidget = m_WidgetComponent->SetWidgetClassReturnWidgetBase(Widget);
			m_WidgetComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		}

	}
}




//////////////////////////////////////////////////////////////////////////
////////////////				测试代码		 	 ////////////////////
void UUISceneComponentBase::TestPlay()
{
	if (!BluprintPath.IsEmpty())
		createUIWidget(BluprintPath);
	else if (userWidget != NULL)
		createUIWidget(userWidget);
}