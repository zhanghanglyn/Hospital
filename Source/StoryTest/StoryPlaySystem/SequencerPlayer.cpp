// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerPlayer.h"

USequencerPlayer::USequencerPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SelfType = STORY_PLAYERTYPE::SEQ_PLAYER;

}

void USequencerPlayer::PlayStateCall(STORY_PLAYSATAE _CurState, TEST_PAGE_DATA _TestData)
{
	UE_LOG(LogTemp, Warning, TEXT("USequencerPlayer Is Worked!"));

	//只处理播放中事件
	if (_CurState == STORY_PLAYSATAE::BEFORE_PLAY)
		M_PlayOverDelegate.ExecuteIfBound(SelfType);
	else if (_CurState == STORY_PLAYSATAE::PLAYING)
	{
		if (!_TestData.SequencerPath.IsEmpty())
			PlaySequencer(_TestData.SequencerPath);
	}
	else if (_CurState == STORY_PLAYSATAE::AFTER_PLAY)
		M_PlayOverDelegate.ExecuteIfBound(SelfType);
		
}

/* 初始化一个Sequencer播放器 */
void USequencerPlayer::Init(UWorld* world)
{
	if (world == nullptr)
		return;
	m_MovieActor = world->SpawnActor<ALevelSequenceActor>();
	//m_sequencePlayer = m_MovieActor->GetSequencePlayer();

	//绑定播放完成委托以及给播放完成委托绑定函数
	//PlayOverDelegate.Clear();
	//PlayOverDelegate.BindUFunction(this, "PlaySequencer");

	//m_sequencePlayer->OnFinished.Add(PlayOverDelegate);
}

void USequencerPlayer::PlaySequencer( FString FullPath )
{
	/* 先加载Sequencer */
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
	CurMovieSequence = AssetLoader.LoadSynchronous<ULevelSequence>(FSoftObjectPath(FullPath)); //<ULevelSequence>(FSoftObjectPath(FullPath));
	m_MovieActor->SetSequence(CurMovieSequence);
	m_sequencePlayer = m_MovieActor->GetSequencePlayer();
	//绑定播放完成委托以及给播放完成委托绑定函数
	if (PlayOverDelegate.IsBound() == false)
	{
		PlayOverDelegate.BindUFunction(this, "PlaySequencerOver");
	}

	m_sequencePlayer->OnFinished.AddUnique(PlayOverDelegate);

	if (m_sequencePlayer != NULL)
	{
		if (m_sequencePlayer->IsPlaying() == true)
			m_sequencePlayer->Stop();

		m_sequencePlayer->Play();
	}
}

void USequencerPlayer::PlaySequencerOver()
{
	UE_LOG(LogTemp , Warning , TEXT(" Play Sequencer Over"));
	M_PlayOverDelegate.ExecuteIfBound(SelfType);
}