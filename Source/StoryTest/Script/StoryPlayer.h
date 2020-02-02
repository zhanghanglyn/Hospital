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

	//变量都声明在这里
public:
	//config文件路径 暂时用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioPath = "StoryTest/Config/";
	//scenarioConfig文件名 暂时用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		FString ScenarioConfigName = "SectionConfig.SectionConfig";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
		ALevelSequenceActor* m_MovieActor;
	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "DataConfig")
	ULevelSequencePlayer* m_sequencePlayer;

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


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 先去取剧本配置表，再去找到对应的剧本
	UFUNCTION(BlueprintCallable)
		void SetScenarioByPathName(FString PathName);

	//跑剧情
	UFUNCTION(BlueprintCallable)
		void StartRun();

	//下一页,用蓝图在后面拖
	UFUNCTION(BlueprintCallable)
		FScenarioDataBase PlayNext();

	/*	根据path路径获取sequencer 并且返回一个播放序列,目前以整个大块sequence来做，
		整个剧本只需要执行一次
	*/
	UFUNCTION(BlueprintCallable)
		void GetScenarioSequencer(FString FullPath);
	
	//判断是否进行事件
	

	//判断音效



	//抛出playerOver事件


};

