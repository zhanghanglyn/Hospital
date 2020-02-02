// Fill out your copyright notice in the Description page of Project Settings.
//@ 张航 备注
/*
	其实也可以不引用FullScreenWidgetBase直接使用UUserwidget这样耦合性低一些，但这里我想让他们配套使用,
	因为个人觉得，是一定要继承的
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Engine/Engine.h"
#include "Script/FullScreenWidgetBase.h"
#include "Script/FullScreenRoot.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMGManager.generated.h"

/**
 * UMG管理类 单例 存放于GameInstance中
 */
UCLASS(BlueprintType , Blueprintable)
class STORYTEST_API UUMGManager : public UObject
{
	GENERATED_BODY()

private:
	//static UUMGManager* Instace;

//实现父类方法相关等
public:
	UUMGManager(const FObjectInitializer& ObjectInitializer);
	
	/*static UUMGManager* GetInstance()
	{
		static UUMGManager* Instace;
		if (Instace == nullptr)
			Instace = NewObject<UUMGManager>();
		return Instace;
	}*/

/************************************************************************/
/*						直接创建至屏幕相关                             */
/************************************************************************/
public:
	//向屏幕添加一个UMG
	UFUNCTION(BlueprintCallable , BlueprintCosmetic, meta = (WorldContext = "WorldContextObject"))
	UFullScreenWidgetBase* CreateScreenWidget(const UObject* WorldContextObject, FString _widgetBlueprintPath, TSubclassOf<UFullScreenWidgetBase> _widgetType, FString _widgetName, int32 _zorder = 0);
	//向屏幕添加一个UMG
	
	template<typename T>
	T* CreateScreenWidget(FString _widgetBlueprintPath,UWorld* _world, TSubclassOf<UFullScreenWidgetBase> _widgetType, FString _widgetName, int32 _zorder = 0)
	{
		if (m_ScreenWidget.Num() > 0 && m_ScreenWidget.Find(_widgetName) != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("this name is aleady in the map!!!!"));
			return nullptr;
		}

		UClass* Temp_Widget = LoadClass<T>(NULL, _widgetBlueprintPath.GetCharArray().GetData());
		if (Temp_Widget != nullptr)
		{
			T* NewWidget = CreateWidget<T>(_world, Temp_Widget);
			if(NewWidget != nullptr)
				(Cast<UFullScreenWidgetBase>(NewWidget))->AddToViewport(_zorder);
			m_ScreenWidget.Add(_widgetName, NewWidget);
			//m_WidgetTypeRelate.Add(_widgetName, _widgetType);
			return NewWidget;
		}
		return nullptr;
	}

	//获取UMG
	UFullScreenWidgetBase* GetScreenWidget(FString _widgetName);
	//获取UMG
	template<typename T1>
	T1* GetScreenWidget(FString _widgetName)
	{
		if (m_ScreenWidget.Num() > 0 && m_ScreenWidget.Find(_widgetName) != nullptr)
		{
			//m_WidgetTypeRelate[1]::Class aa = Cast< m_WidgetTypeRelate[1]>(*m_ScreenWidget.Find(_widgetName));
			return Cast<T1>(*m_ScreenWidget.Find(_widgetName));
		}
		return nullptr;
	}

	//获取对应名字的UMGType
	TSubclassOf<UFullScreenWidgetBase> GetWidgetTypeByName(FString _widgetName);

	//清除Widget
	void ClearWidget(FString _widgetName);
	void ClearAll();

	//设置点击GameMode或是PlayerMode事件
	void SetInputMode(UWorld* _world, FInputModeDataBase& InData);

protected:
	//根据名字记录Widget
	UPROPERTY()
	TMap<FString, UFullScreenWidgetBase*> m_ScreenWidget;
	//记录每个名字对应的类型，用来转换  //暂时感觉没什么卵用 10.22
	TMap<FString, TSubclassOf<UFullScreenWidgetBase>> m_WidgetTypeRelate;

/************************************************************************/
/*						创建至GameInstance相关                          */
/************************************************************************/
public:
	//在此向GameInstance GameViewportClient中加入根节点UI 该UI不会被清除
	void CreateInstanceRootWidget(UGameInstance* GameInstance);

	//创建INstanceUMG
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (WorldContext = "WorldContextObject"))
	UFullScreenWidgetBase* CreateInstanceWidget(const UObject* WorldContextObject ,FString _widgetBlueprintPath,int32 _zorder = 0);
	UFullScreenWidgetBase* CreateInstanceWidget(UWorld* _world, FString _widgetBlueprintPath, int32 _zorder = 0);

	//根据UID查找某个UMD
	UFullScreenWidgetBase* GetInsUMGByUID(FString UID);
	//删除全屏UMG
	void DeleteInsUMGWidget(FString UID);
	//删除全屏UMG
	void DeleteInsUMGWidget( UFullScreenWidgetBase* widget);
	//清空全屏UMG
	void ClearInsUMG();

protected:
	//根节点的widget
	UPROPERTY()
	UFullScreenRoot* m_RootWidget;
	//存储在根节点上的UI的Array
	UPROPERTY()
	TArray<UFullScreenWidgetBase*> m_InsWidgetList;
	//用来记录Widget名字对应的List中的Index  //19.11.4 由于引擎莫名其妙的清空行为，重构一次
	//TMap<FString, int32> m_InsWidgetIndexList;

	UPROPERTY()
	FVector2D m_ViewPortSize;

};
