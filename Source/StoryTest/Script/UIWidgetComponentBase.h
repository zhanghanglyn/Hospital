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

//��չ ��д���෽�����
public:
	//����UI���ҷ���USERWIDGET
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

	//��չtick
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);


//��̬�������
public:
	/**
	����һ��UIWIDGETCOMPONENT�������ע��
	*param:outer	 :    ��SceneComponent
	*param:bAttachTo :	  �������Ƿ�Attach�����ڵ���
	*/
	static UUIWidgetComponentBase* CreateUIWidgetBase(USceneComponent *outer, bool _bAttachTo, bool _bToCamera);

//������ͼ�����õı������
public:
	//�Ƿ��������
	UPROPERTY( EditAnywhere ,BlueprintReadWrite , Category = "GamePlayData")
	bool bToCamera;

//�ڲ���Ա����
protected:


//GamePlay�������
public:
	//ͨ������һ��UUSERWIDGET���StaticClass �ڹ��캯���д���һ��UI
	UUserWidget* SetUI(UClass *_userWidget);
	//ͨ��ʹ����ͼ·��������һ��UI
	UUserWidget* SetUI(FString _bluePrintPath);
	//ͨ��ʹ����ͼ·��������һ��UI,ģ�庯��
	template< typename T1 >
	T1* SetUI(FString _bluePrintPath)
	{
		UClass* Temp_Widget = LoadClass<T1>(NULL, _bluePrintPath.GetCharArray().GetData());
		T1* tempWidget = SetWidgetClassReturnWidgetBase<T1>(Temp_Widget);

		return Cast<T1>(tempWidget);
	}

	//��ȡWIDGET
	UUserWidget* GetWidget();

	//�������λ��
	void SetUIPosition(FVector _position );
	void SetUIRotation(FRotator _rotator);
};
