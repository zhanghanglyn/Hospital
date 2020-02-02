// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StoryPlaySystem.generated.h"

/*enum STORY_PLAYSATAE
{
	NONE,
	BEFORE_PLAY,
	PLAYING,
	AFTER_PLAY,
};*/

/* 自动循环添加 */
/*STORY_PLAYSATAE &operator++(STORY_PLAYSATAE &rs)
{
	int32 i = rs;
	i++;
	if( i > AFTER_PLAY)
		rs = STORY_PLAYSATAE::NONE;
	else
		rs = (enum STORY_PLAYSATAE)(i);
	return rs;
};*/

//播放器类型枚举
UENUM(BlueprintType)
enum class STORY_PLAYERTYPE : uint8
{
	NONE = 0,
	EVENT_PLAYER = 1,
	SEQ_PLAYER = 2,
	SELECT_PLAYER = 3,
};

/*定义播放类型枚举*/
enum STORY_PLAYSATAE
{
	NONE,
	BEFORE_PLAY,
	PLAYING,
	AFTER_PLAY,
};

/* 测试发送数据结构 */
struct TEST_PAGE_DATA
{
public:
	FString SequencerPath;
};

class UStoryPlayerBase;

/**
 * 剧情播放系统初版，播放剧情分为播放前，播放时，播放后三种情况，三种情况均会调用委托,
 * 当所有委托返回true时，则会进行下一步
 * 蓝图绑定委托，自己获取委托调用蓝图函数绑定即可，绑定后还需要手动调用增加计数的函数
 */
UCLASS()
class UStoryPlaySystem : public UObject
{
	GENERATED_BODY()

public:
	//事件播放器委托  ,参数为当前播放状态是开始前，开始时还是开始后
	DECLARE_MULTICAST_DELEGATE_TwoParams(EventPlayer, STORY_PLAYSATAE , TEST_PAGE_DATA)
	EventPlayer EventPlayerDelegate;
	//Sequencer委托
	DECLARE_MULTICAST_DELEGATE_TwoParams(SequencerPlayer, STORY_PLAYSATAE , TEST_PAGE_DATA)
	SequencerPlayer SequencerPlayerDelegate;
	//Select委托
	DECLARE_MULTICAST_DELEGATE_TwoParams(SelectPlayer, STORY_PLAYSATAE , TEST_PAGE_DATA)
	SelectPlayer SelectPlayerDelegate;

	static STORY_PLAYSATAE AddState(STORY_PLAYSATAE &out)
	{
		int32 i = out;
		i++;
		if (i > STORY_PLAYSATAE::AFTER_PLAY)
			out = STORY_PLAYSATAE::NONE;
		else
			out = (enum STORY_PLAYSATAE)(i);
		return out;
	};

public:
	//外部还是调用这个吧 获取需要设置的委托
	TMulticastDelegate<void, STORY_PLAYSATAE , TEST_PAGE_DATA>& GetDelegate(STORY_PLAYERTYPE _TYPE);
	
	//传入播放器，为其设置一个播放完毕回调
	void SetCallBack(UStoryPlayerBase *PlayerBase);

	//增加委托计数
	UFUNCTION(BlueprintCallable)
	void AddPlayerCount(STORY_PLAYERTYPE _TYPE);
	UFUNCTION(BlueprintCallable)
	void ReducePlayerCount(STORY_PLAYERTYPE _TYPE);
	//清空委托
	void ClearDelegate();

/* 以下为失败的测试方法，后续再研究吧 */
protected:
	// 不用这个！！ 外部调用，向故事播放器注册播放事件,不支持蓝图 param : _Delegate 事件播放器的委托，会在上绑定一个事件完成的回调函数
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	void RegistPlayer(STORY_PLAYERTYPE PlayerType, UserClass* InUserObject, typename TMemFunPtrType<true, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars)
		//void RegistPlayer(STORY_PLAYERTYPE PlayerType, UserClass* InUserObject, typename TMemFunPtrType<false, UserClass, void(ParamTypes...)>::Type InFunc)
	{
		//绑定一个播放完毕的委托
		(Cast<UStoryPlayerBase>(InUserObject))->M_PlayOverDelegate.BindUObject(this, &UStoryPlaySystem::PlayerDelegateCallBack);

		switch (PlayerType)
		{
		case STORY_PLAYERTYPE::EVENT_PLAYER:
			RegistEventPlayer(InUserObject, InFunc);
			break;
		case STORY_PLAYERTYPE::SEQ_PLAYER:
			RegistSequencerPlayer(InUserObject, InFunc);
			break;
		case STORY_PLAYERTYPE::SELECT_PLAYER:
			RegistSelectPlayer(InUserObject, InFunc);
			break;
		}
	}
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	void RegistEventPlayer(UserClass* InUserObject, typename TMemFunPtrType<true, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars)
	{
		EventPlayerDelegate.AddUObject(InUserObject, InFunc);
		AddPlayerCount(EVENT_PLAYER);
	};
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	void RegistSequencerPlayer(UserClass* InUserObject, typename TMemFunPtrType<true, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars)
	{
		SequencerPlayerDelegate.AddUObject(InUserObject, InFunc);
		AddPlayerCount(SEQ_PLAYER);
	};
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	void RegistSelectPlayer(UserClass* InUserObject, typename TMemFunPtrType<true, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc, VarTypes... Vars)
	{
		SelectPlayerDelegate.AddUObject(InUserObject, InFunc);
		AddPlayerCount(SELECT_PLAYER);
	};

private:
	//绑定事件计数,三种
	int32 EventPlayerCount = 0;
	int32 SequencerPlayerCount = 0;
	int32 SelectPlayerCount = 0;
	//当前执行完返回的各播放计数
	int32 EventPlayerCallCount = 0;
	int32 SequencerPlayerCallCount = 0;
	int32 SelectPlayerCallCount = 0;
	//当前播放状态
	STORY_PLAYSATAE CurPlayState = STORY_PLAYSATAE::NONE;

	//当前页数据
	FString CurData;
	//是否有结束标识
	bool bFinishTag;

	//测试数据
	TEST_PAGE_DATA* test_Data;

/*
	正式播放以及获取数据等相关
*/
public:
	/* 向树请求数据 */
	UFUNCTION(BlueprintCallable)
	void GetPlayData();

	/* 播放剧情，向所有委托进行分发 */
	UFUNCTION(BlueprintCallable)
	void Play();

	/* 收到分发的委托回调后，增加相应的计数器 */
	UFUNCTION(BlueprintCallable)
	void PlayerDelegateCallBack(STORY_PLAYERTYPE _TYPE);
	
	/* 结束本次播放 */
	void FinishPlay();

/*内部用计算函数*/
protected:
	/* 判断是否已经播放完毕是切换下一状态或是切换下一页面 */
	void CheckBePlayNext();

	/* 切换播放下一状态 */
	void PlayNextState();

	/* 清空当页数据并且播放下页 */
	void ResetParamPlayNext();
};
