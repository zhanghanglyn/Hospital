#pragma once

#include "CoreMinimal.h"
#include "CustomAssetEditor/EventTreeSystem/NodesController/NodeControllerBase.h"
#include "CustomAssetEditor/EventTreeSystem/NodesController/Mgr/NodeControllerMgr.h"
#include "EventTreeMgr.generated.h"

/*
一个节点仅有一个控制器，但是一个控制器可能对应有多个组件？
*/
UCLASS()
class CUSTOMASSETEDITOR_API UEventTreeMgr : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(DEventPlayOverCallDelegate, int32)
	DEventPlayOverCallDelegate EventPlayOverCallDelegate;

public:
	UEventTreeMgr(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {};

	//根据路径获取Tree资源
	bool GetTreeAsset( FString Path );

	//开始循环树,并且设置播放完成的委托
	void StartRun(UObject* OverCallObj, FName _UFunctionName);
	
	//开始下一个节点
	void PlayNext();

	void Finish();

	//完成本次循环清除数据等
	void Clear();

	//每个Controller完成时的回调
	UFUNCTION()
	void ControllerOverCallBack(FName ControllerCategory);


	/****  给3C使用接口  ****/
	/*
		开始触摸
		@PARAM:
		Controller3C : 3C控制器指针
		TouchObj	 : 射到的物体
		XXXX		 : 当前的按键
	*/
	UFUNCTION(BlueprintCallable)
	void StartTouch( UObject* Controller3C , UObject* TouchObj , FKey key );

	/*
		移动鼠标进入互动物体事件
		@PARAM:
		Controller3C : 3C控制器指针
		TouchObj	 : 射到的物体
	*/
	UFUNCTION(BlueprintCallable)
	void Hover(UObject* Controller3C, UObject* TouchObj);

	/*
		移动鼠标离开互动物体事件
		@PARAM:
		Controller3C : 3C控制器指针
		TouchObj	 : 射到的物体
	*/
	UFUNCTION(BlueprintCallable)
	void TouchLeave(UObject* Controller3C, UObject* TouchObj);

	/*
		3C初始化时调用，将自身指针设置给EventMgr
	*/
	UFUNCTION(BlueprintCallable)
	void InitPlayerController(UObject* InController3C)
	{
		PlayerController3C = InController3C;
	};

	/****  给3C使用接口END  ****/
protected:
	//从Array中查找所有的链接节点，并且为其生成控制器，将控制器注册到本次节点事件中
	void CreateController(TArray<class UEdGraphNode*> LinkNodes);

	//判断是否有END节点
	bool CheckEndNodes(TArray<class UEdGraphNode*> LinkNodes);

protected:
	//Asset资源
	UPROPERTY()
	class UMyCustomAsset* TreeAsset;

	/** Graph for Tree */
	UPROPERTY()
	class UScenarioGraph* TreeGraph;

	/* 当前走到的节点 */
	UPROPERTY()
	class UScenarioNodeNormal* CurNode;

	UPROPERTY()
	class URootNodes* RootNode;

	//本次事件节点运行中的Controller
	UPROPERTY()
	TArray<class UNodeControllerBase* > NodeControllers;

	//装配节点的Controller，该数据结构并不会在每一个Action完成时清除
	UPROPERTY()
	TArray<class UNodeControllerBase* > AssembleNodeControllers;

	/* 事件中也会有Choose选项，对应不同的选择结果？暂时写作-1 */
	UPROPERTY()
	int32 CurChoose = -1;

	/* 3C控制器实例 */
	UPROPERTY()
	UObject* PlayerController3C;
};