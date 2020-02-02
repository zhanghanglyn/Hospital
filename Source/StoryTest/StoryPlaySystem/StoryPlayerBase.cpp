// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryPlayerBase.h"

UStoryPlayerBase::UStoryPlayerBase(const FObjectInitializer& ObjectInitializer)
{
	SelfType = STORY_PLAYERTYPE::NONE;
}

/* 如果自身类型为NONE，不注册 */
void UStoryPlayerBase::RegistToSystem(UStoryPlaySystem* PlayerSystem)
{
	if (PlayerSystem == nullptr || SelfType == STORY_PLAYERTYPE::NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find StoryPlayer System !!!"));
		return;
	}
		
	(PlayerSystem->GetDelegate(SelfType)).AddUObject(this, &UStoryPlayerBase::PlayStateCall);
	PlayerSystem->SetCallBack(this);
}

void UStoryPlayerBase::PlayStateCall(STORY_PLAYSATAE _CurState, TEST_PAGE_DATA _TestData)
{
	UE_LOG(LogTemp, Warning, TEXT("Parent PlayerBase Is Worked!"));
	M_PlayOverDelegate.ExecuteIfBound(SelfType);
}