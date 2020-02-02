// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryPlaySystem.h"
#include "StoryPlaySystem/StoryPlayerBase.h"
/************************************************************************/
/*                       设置委托等相关                            */
/************************************************************************/
TMulticastDelegate<void, STORY_PLAYSATAE, TEST_PAGE_DATA>& UStoryPlaySystem::GetDelegate(STORY_PLAYERTYPE _TYPE)
{
	switch (_TYPE)
	{
	case STORY_PLAYERTYPE::EVENT_PLAYER:
		return EventPlayerDelegate;
		break;
	case STORY_PLAYERTYPE::SEQ_PLAYER:
		return SequencerPlayerDelegate;
		break;
	case STORY_PLAYERTYPE::SELECT_PLAYER:
		return SelectPlayerDelegate;
		break;
	}

	return EventPlayerDelegate;
}
/* 注册回调的同时，增加计数 */
void UStoryPlaySystem::SetCallBack(UStoryPlayerBase *PlayerBase)
{
	if (PlayerBase == nullptr || PlayerBase->SelfType == STORY_PLAYERTYPE::NONE)
		return;

	//绑定一个播放完毕的委托
	PlayerBase->M_PlayOverDelegate.BindUObject(this, &UStoryPlaySystem::PlayerDelegateCallBack);
	AddPlayerCount(PlayerBase->SelfType);

}


/* 需要在此解析是否为结束页，如果是结束页，结束本次播放 */
void UStoryPlaySystem::GetPlayData()
{

	//先添加一个测试数据
	test_Data = new TEST_PAGE_DATA();
	test_Data->SequencerPath = "/Game/StoryTest/Sequencer/NewLevelSequence.NewLevelSequence";

}

/* 正式播放剧情 */
void UStoryPlaySystem::Play()
{
	GetPlayData();
	PlayNextState();
}

/*  */
void UStoryPlaySystem::FinishPlay()
{

}

/* 播放下一状态，一开始状态为0，之后逐渐增加，当最大后回归0（清空当页时回归0） */
void UStoryPlaySystem::PlayNextState()
{
	//重置计数器
	EventPlayerCallCount = 0;
	SequencerPlayerCallCount = 0;
	SelectPlayerCallCount = 0;

	//将当前状态加一
	CurPlayState = UStoryPlaySystem::AddState(CurPlayState);
	/* 如果相加后为0，则说明已经完成了一个轮询,播放下一页 */
	if (CurPlayState == STORY_PLAYSATAE::NONE)
	{
		ResetParamPlayNext();
		return;
	}

	EventPlayerDelegate.Broadcast(CurPlayState, *test_Data);
	SequencerPlayerDelegate.Broadcast(CurPlayState, *test_Data);
	SelectPlayerDelegate.Broadcast(CurPlayState, *test_Data);
}

/* 重置计时器，重新获取下页数据，如果有结束标识，则在此结束
*/
void UStoryPlaySystem::ResetParamPlayNext()
{
	if (bFinishTag == false)
		//GetPlayData();
		Play();
	else
		FinishPlay();
}

/* 外部注册事件调用该函数来标识完成本次play */
void UStoryPlaySystem::PlayerDelegateCallBack(STORY_PLAYERTYPE _TYPE)
{
	switch (_TYPE)
	{
	case STORY_PLAYERTYPE::EVENT_PLAYER:
		EventPlayerCallCount++;
		break;
	case STORY_PLAYERTYPE::SEQ_PLAYER:
		SequencerPlayerCallCount++;
		break;
	case STORY_PLAYERTYPE::SELECT_PLAYER:
		SelectPlayerCallCount++;
		break;
	}

	CheckBePlayNext();
}

void UStoryPlaySystem::CheckBePlayNext()
{
	if (EventPlayerCallCount >= EventPlayerCount && SequencerPlayerCallCount >= SequencerPlayerCount && SelectPlayerCallCount >= SelectPlayerCount)
		PlayNextState();
}

/* 接受回调并增加计数 */
void UStoryPlaySystem::AddPlayerCount(STORY_PLAYERTYPE _TYPE)
{
	switch (_TYPE)
	{
	case STORY_PLAYERTYPE::EVENT_PLAYER:
		EventPlayerCount++;
		break;
	case STORY_PLAYERTYPE::SEQ_PLAYER:
		SequencerPlayerCount++;
		break;
	case STORY_PLAYERTYPE::SELECT_PLAYER:
		SelectPlayerCount++;
		break;
	default:
		break;
	}
}

void UStoryPlaySystem::ReducePlayerCount(STORY_PLAYERTYPE _TYPE)
{
	switch (_TYPE)
	{
	case STORY_PLAYERTYPE::EVENT_PLAYER:
		EventPlayerCount--;
		break;
	case STORY_PLAYERTYPE::SEQ_PLAYER:
		SequencerPlayerCount--;
		break;
	case STORY_PLAYERTYPE::SELECT_PLAYER:
		SelectPlayerCount--;
		break;
	default:
		break;
	}
}

void UStoryPlaySystem::ClearDelegate()
{
	EventPlayerDelegate.Clear();
	SequencerPlayerDelegate.Clear();
	SelectPlayerDelegate.Clear();
}