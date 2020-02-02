// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Core/Public/Misc/Paths.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "StoryTest/DataUtil/ScenarioDataBase.h"
#include "StoryTest/DataUtil/ScenarioScenarioConfig.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Engine/Classes/Engine/SkeletalMesh.h"
#include "StoryPlayer.generated.h"

UCLASS()
class STORYTEST_API AStoryPlayer : public AActor
{
	GENERATED_BODY()
	
	
public:
	// Sets default values for this actor's properties
	AStoryPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//����������������
public:
	//config�ļ�·�� ��ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioPath = "StoryTest/Config/";
	//scenarioConfig�ļ��� ��ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioConfigName = "SectionConfig.SectionConfig";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		ALevelSequenceActor* m_MovieActor;
	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "DataConfig")
	ULevelSequencePlayer* m_sequencePlayer;

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


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��ȥȡ�籾���ñ���ȥ�ҵ���Ӧ�ľ籾
	UFUNCTION(BlueprintCallable)
		void SetScenarioByPathName(FString PathName);

	//�ܾ���
	UFUNCTION(BlueprintCallable)
		void StartRun();

	//��һҳ,����ͼ�ں�����
	UFUNCTION(BlueprintCallable)
		FScenarioDataBase PlayNext();

	/*	����path·����ȡsequencer ���ҷ���һ����������,Ŀǰ���������sequence������
		�����籾ֻ��Ҫִ��һ��
	*/
	UFUNCTION(BlueprintCallable)
		void GetScenarioSequencer(FString FullPath);
	
	//�ж��Ƿ�����¼�
	

	//�ж���Ч



	//�׳�playerOver�¼�


};

