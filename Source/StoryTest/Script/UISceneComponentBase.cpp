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

//ͨ������̳���UUserWidget��C++static��������ģ�壬���� UUIWidgetComponentBase::StaticClass()
UUISceneComponentBase::UUISceneComponentBase(UClass *userWidget)
{

	PrimaryComponentTick.bCanEverTick = true;

	createUIWidget(userWidget);
}

//ͨ�������UI���͵ȳ�ʼ��,����TEXT("WidgetBlueprint'/Game/UI/widget/HpBar.HpBar_C'")
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
////////////////////	 ��д��չ���෽�����	  ////////////////////////////
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
////////////////////	 ��д��չ���෽�����END	  ////////////////////////
/////////////////////////////////////////////////////////////////////////////




// Called every frame
void UUISceneComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//ͨ������һ��UUSERWIDGET���StaticClass �ڹ��캯���д���һ��UI
void UUISceneComponentBase::createUIWidget(UClass *_userWidget)
{
	//��ʼ��widgetComponent
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

//ͨ��ʹ����ͼ·��������һ��UI
void UUISceneComponentBase::createUIWidget(FString _bluePrintPath)
{
	//��ʼ��widgetComponent
	if (m_WidgetComponent == NULL)
	{
		//CreateDefaultSubobjectֻ���ڹ��캯���д���������
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
////////////////				���Դ���		 	 ////////////////////
void UUISceneComponentBase::TestPlay()
{
	if (!BluprintPath.IsEmpty())
		createUIWidget(BluprintPath);
	else if (userWidget != NULL)
		createUIWidget(userWidget);
}