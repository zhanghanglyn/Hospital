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

//��������Լ��¼�
public :
	// as a click event,play next or event //
	UFUNCTION(BlueprintCallable)
	void Click();

	void TurnRate(float Rate);

//��������
public:

	//config�ļ�·�� ��ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioPath = "StoryTest/Config/";
	//scenarioConfig�ļ��� ��ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioConfigName = "SectionConfig.SectionConfig";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		ALevelSequenceActor* m_MovieActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		ULevelSequencePlayer* m_sequencePlayer;

//ϵͳ����
public:
	
	// ��ȥȡ�籾���ñ���ȥ�ҵ���Ӧ�ľ籾
	UFUNCTION(BlueprintCallable)
		void SetScenarioByPathName(FString PathName);

	//�ܾ���
	UFUNCTION(BlueprintCallable)
		void StartRun();

	//��һҳ,����ͼ�ں�����
	UFUNCTION(BlueprintCallable)
		void PlayNext();

	//ÿһҳ��ÿһ���������֮�󶼻�ͳһ����һ�θûص�����
	void CallBackByEveryPageAction(int32 _page = -1);

//����ֳ�ÿһ���¼����
public:
	/*	����path·����ȡsequencer ���ҷ���һ����������,Ŀǰ���������sequence������
		�����籾ֻ��Ҫִ��һ��
	*/
	UFUNCTION(BlueprintCallable)
		void GetScenarioSequencer(FString FullPath);
	UFUNCTION(BlueprintCallable)
		void PlaySequence(FScenarioDataBase cur_page);
	//Sequenver ������ϻص�
	UFUNCTION()
		void SequencePlayOverCallBack();

	//�����Ƿ�ر��������
	UFUNCTION(BlueprintCallable)
	void SetBeControlCamera(bool _bControl);

	//Sequencer Click ,default Click
	void SequencerClick();

	//play event by event id
	UFUNCTION(BlueprintCallable)
		void PlayEvent(FString _eventId);

	//Pick Event click
	void PickEventClick();
	
	//���ݵ��������ִ���������
	int32 DealDataByClickName(FString _hitName);

	/************************************************************************/
	/*	             ����ѡ��												*/
	/************************************************************************/
	UFUNCTION(BlueprintCallable)
	void PlaySelection(FScenarioDataBase cur_page);

//�籾�ں���
protected:
	//��ǰ�籾��map����
	TMap< int32, FScenarioDataBase> m_curData;
	//��ǰ����ҳ��
	int32 m_curPage;
	//��ǰ�籾SequencerPath
	FString m_sequencePath;
	//��ǰ�籾��Sequencer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
	ULevelSequence* IntroMovieSequence;

	//��ǰ�����UI
	USelectWidget* m_SelectUI;

	//����һ������¼�ί��,����һ����ǰ�������
	DECLARE_DELEGATE(ClickDelegate);
	ClickDelegate m_clickDelegate;

	//���Ž���ʱ�Ļص�ί��
	FScriptDelegate finishDelegate;

	//ѡ��������ص�����
	void SelectOneCall(SelectParam param);

protected:
	//���Ժ������������һ����������������˻���Component�����壬�������Action
	UFUNCTION(BlueprintCallable)
	void CheckActiveObjAction();

//�ڲ��������ݺ�����
protected:
	UPROPERTY(BlueprintReadOnly, Category = "PageInfo")
		bool bSequencerOver = false;
	UPROPERTY(BlueprintReadOnly, Category = "PageInfo")
		bool bEventOver = false;
	UPROPERTY(BlueprintReadOnly, Category = "PageInfo")
		bool bSelectOver = false;

	//��ʼ������¼�
	UFUNCTION(BlueprintCallable)
		void InitializePage();

	//���õ�ǰ���ΪGAMEONLY
	UFUNCTION()
		void SetInputModeGameOnly();
	UFUNCTION()
		void SetInputModeUIOnly();


//���Ա�����
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
