// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "Core/Public/Misc/Paths.h"
#include "Engine/StreamableManager.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "Engine/Classes/Camera/PlayerCameraManager.h"
#include "Engine/AssetManager.h"
#include "StoryTest/DataUtil/EventEnum.h"
#include "StoryTest/DataUtil/ScenarioDataBase.h"
#include "StoryTest/DataUtil/ScenarioScenarioConfig.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "StoryTest/Script/SelectWidget.h"
#include "GameFramework/PlayerController.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "UMG/Public/Components/GridSlot.h"
#include "StoryTest/Script/UIWidgetComponentBase.h"
#include "StoryTest/DataUtil/EventEnum.h"
#include "Classes/Engine/StaticMesh.h"
#include "Engine/Classes/Engine/StaticMeshActor.h"
#include "FullUMG/UMGManager.h"
#include "Script/FullScreenWidgetBase.h"
#include "WidgetUI/FullScreenTest.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "StoryTest/GameMgr/StoryGameInstance.h"
#include "Classes/GameFramework/Character.h"
#include "StoryTest/TestTouchUI.h"
#include "StoryPlaySystem/StoryPlayerBase.h"
#include "StoryPlaySystem/SequencerPlayer.h"
#include "StoryPlayerPawn.generated.h"

UCLASS()
class STORYTEST_API AStoryPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStoryPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//点击函数以及事件
public :
	// as a click event,play next or event //
	UFUNCTION(BlueprintCallable)
	void Click();

	void TurnRate(float Rate);

//变量声明
public:

	//config文件路径 暂时用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioPath = "StoryTest/Config/";
	//scenarioConfig文件名 暂时用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioConfigName = "SectionConfig.SectionConfig";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		ALevelSequenceActor* m_MovieActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		ULevelSequencePlayer* m_sequencePlayer;

//系统函数
public:
	
	// 先去取剧本配置表，再去找到对应的剧本
	UFUNCTION(BlueprintCallable)
		void SetScenarioByPathName(FString PathName);

	//跑剧情
	UFUNCTION(BlueprintCallable)
		void StartRun();

	//下一页,用蓝图在后面拖
	UFUNCTION(BlueprintCallable)
		void PlayNext();

	//每一页的每一个动作完成之后都会统一调用一次该回调函数
	void CallBackByEveryPageAction(int32 _page = -1);

//具体分成每一个事件相关
public:
	/*	根据path路径获取sequencer 并且返回一个播放序列,目前以整个大块sequence来做，
		整个剧本只需要执行一次
	*/
	UFUNCTION(BlueprintCallable)
		void GetScenarioSequencer(FString FullPath);
	UFUNCTION(BlueprintCallable)
		void PlaySequence(FScenarioDataBase cur_page);
	//Sequenver 播放完毕回调
	UFUNCTION()
		void SequencePlayOverCallBack();

	//设置是否关闭相机剪裁
	UFUNCTION(BlueprintCallable)
	void SetBeControlCamera(bool _bControl);

	//Sequencer Click ,default Click
	void SequencerClick();

	//play event by event id
	UFUNCTION(BlueprintCallable)
		void PlayEvent(FString _eventId);

	//Pick Event click
	void PickEventClick();
	
	//根据点击后的名字处理下数据
	int32 DealDataByClickName(FString _hitName);

	/************************************************************************/
	/*	             处理选项												*/
	/************************************************************************/
	UFUNCTION(BlueprintCallable)
	void PlaySelection(FScenarioDataBase cur_page);

//剧本内函数
protected:
	//当前剧本的map数据
	TMap< int32, FScenarioDataBase> m_curData;
	//当前播放页数
	int32 m_curPage;
	//当前剧本SequencerPath
	FString m_sequencePath;
	//当前剧本的Sequencer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
	ULevelSequence* IntroMovieSequence;

	//当前界面的UI
	USelectWidget* m_SelectUI;

	//声明一个点击事件委托,返回一个当前点击类型
	DECLARE_DELEGATE(ClickDelegate);
	ClickDelegate m_clickDelegate;

	//播放结束时的回调委托
	FScriptDelegate finishDelegate;

	//选项的三个回调函数
	void SelectOneCall(SelectParam param);

protected:
	//测试函数，点击发射一条射线如果是设置了互动Component的物体，则调用其Action
	UFUNCTION(BlueprintCallable)
	void CheckActiveObjAction();

//内部处理数据函数等
protected:
	UPROPERTY(BlueprintReadOnly, Category = "PageInfo")
		bool bSequencerOver = false;
	UPROPERTY(BlueprintReadOnly, Category = "PageInfo")
		bool bEventOver = false;
	UPROPERTY(BlueprintReadOnly, Category = "PageInfo")
		bool bSelectOver = false;

	//初始化完成事件
	UFUNCTION(BlueprintCallable)
		void InitializePage();

	//设置当前点击为GAMEONLY
	UFUNCTION()
		void SetInputModeGameOnly();
	UFUNCTION()
		void SetInputModeUIOnly();


//测试变量等
public:
	UUIWidgetComponentBase *WidgetComponent;

	UFUNCTION(BlueprintCallable)
	void TestFunc();

	UFUNCTION(BlueprintCallable)
	void TestFuncTwo();

	UFUNCTION(BlueprintCallable)
	void TestFuncThree();

	UPROPERTY()
	UTestTouchUI *temp_touchUI;

	UPROPERTY()
		FString testUID;
};
