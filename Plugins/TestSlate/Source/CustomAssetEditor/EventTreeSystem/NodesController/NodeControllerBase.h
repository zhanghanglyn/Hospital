#pragma once
#include "CoreMinimal.h"
#include "EditTabsUtil.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"
#include "ScenarioNodeNormal.h"
#include "NodeControllerBase.generated.h"

/*节点事件控制器的基类，在循环每一个Node的时候，会为Node装配一个控制器，即用即删*/
UCLASS()
class UNodeControllerBase : public UObject
{
	GENERATED_BODY()

public:
	//当前状态执行完成回调委托
	DECLARE_DELEGATE_OneParam(ControllerOverDelegate, FName)
	ControllerOverDelegate OverDelegate;

public:
	UNodeControllerBase(const FObjectInitializer& ObjectInitializer){
		BControlOver = false;
	};

	//根据节点参数进行组件的装配 20.1.22 新添加DynamicParam 根据需求自己处理，会传入EventTreeMgr
	virtual void InitController(UScenarioNodeNormal* InNode , UObject* DynamicParam) {
		CurNode = InNode;
		Init(DynamicParam);
	};

	//接受到Component的已经完成的回调
	virtual void ComponentsFinishCallBack(FName Paramm);

	//向Mgr发送本次节点已完成
	virtual void ControllerFinish();

	//根据数据进行控制器的初始化
	virtual void Init(UObject* DynamicParam) {};
	//清理自身
	virtual void Clear();
	//在下一页的时候清理自身
	virtual void ClearNextPage();
protected:


public:
	//自身的Controller类型
	FName ControllerCategory = FControllerCategoryUtil::ControllerBase;
	//本次互动是否完成
	bool BControlOver = false;
	//当前Mgr绑定的Node
	UPROPERTY()
	UScenarioNodeNormal* CurNode;

protected:

	//根据Node数据创建的Components
	UPROPERTY()
	UEventComponentBase* ComponentBase;

	//存放Components上的Actor
	UPROPERTY()
	AActor* ComponentNodeActor;
};