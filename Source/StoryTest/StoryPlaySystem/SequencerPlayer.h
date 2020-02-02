// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StoryPlaySystem/StoryPlayerBase.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Engine/AssetManager.h"
#include "SequencerPlayer.generated.h"

/**
 * 
 */
UCLASS()
class USequencerPlayer : public UStoryPlayerBase
{
	GENERATED_BODY()
	
public:
	USequencerPlayer(const FObjectInitializer& ObjectInitializer);

	/* 播放器播放时注册的调用事件委托 , 参数为当前播放状态*/
	virtual void PlayStateCall(STORY_PLAYSATAE _CurState, TEST_PAGE_DATA _TestData) override;

	/* 初始化Sequencer播放器等等，后期要优化，不需要传这个WORLD */
	void Init(UWorld* world);
protected:
	/* 正式播放sequencer */
	void PlaySequencer(FString FullPath);
	/* Sequencer播放完毕 */
	UFUNCTION()
	void PlaySequencerOver();

protected:
	/* Actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequencer")
	ALevelSequenceActor* m_MovieActor;
	/* Sequencer播放器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
	ULevelSequencePlayer* m_sequencePlayer;

	/* 当前Sequencer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataConfig")
	ULevelSequence* CurMovieSequence;

	/* 播放完毕的委托 */
	FScriptDelegate PlayOverDelegate;
};
