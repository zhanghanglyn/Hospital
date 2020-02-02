// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StoryPlaySystem/StoryPlaySystem.h"
#include "StoryPlayerBase.generated.h"

/**
 * 剧情播放系统中小播放器的基类，所有播放器例如事件播放器Sequencer播放器都继承该类
 */
UCLASS()
class UStoryPlayerBase : public UObject
{
	GENERATED_BODY()

	//当前状态执行完成回调委托
	DECLARE_DELEGATE_OneParam(PlayOverDelegate, STORY_PLAYERTYPE)

public:
	UStoryPlayerBase(const FObjectInitializer& ObjectInitializer);

	/* 想播放系统注册自身 */
	virtual void RegistToSystem( UStoryPlaySystem* PlayerSystem );

	/* 播放器播放时注册的调用事件委托 , 参数为当前播放状态*/
	virtual void PlayStateCall(STORY_PLAYSATAE _CurState , TEST_PAGE_DATA _TestData);

protected:
	
public:
	STORY_PLAYERTYPE SelfType = STORY_PLAYERTYPE::NONE; //当前自身播放器类型
	STORY_PLAYSATAE CurState = STORY_PLAYSATAE::NONE;  //当前播放状态

	/* 当完成本次状态事件后，向PlayerSystem发出的回调 ， 参数为自身类型*/
	PlayOverDelegate M_PlayOverDelegate;

};
