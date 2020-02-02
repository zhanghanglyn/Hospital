#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Data/ComponentNodeDataBase.h"
#include "EditTabsUtil.h"
#include "EventComponentBase.generated.h"

/* 事件Component的基类 */
UCLASS(ClassGroup = (EventTreeComponent) , meta = (BlueprintSpawnableComponent))
class CUSTOMASSETEDITOR_API UEventComponentBase : public USceneComponent
{
	GENERATED_BODY()

public:
	//当前状态执行完成回调委托
	DECLARE_DELEGATE_OneParam(ComponentFinishOverDelegate, FName)
	ComponentFinishOverDelegate OverDelegate;

public:
	UEventComponentBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
		
	};
	UEventComponentBase() {};

	/* 基类函数，回调自身等 */
	//自身播放完毕后回调结束
	virtual void FinishCallBack();

	//把自身清除
	virtual void Clear() {
		OverDelegate.Unbind();
	};

	/*END 基类函数，回调自身等 */

	//Data是由外部的Obj进行保存的,SetData还应该进行本Component的初始化  20.1.19修改为使用同一个类型的ComponentBase来传递参数
	//virtual void SetData(UComponentNodeDataBase* Data) {};
	virtual void SetData(UEventComponentBase* Data) {};

	//Action为外部调用函数
	virtual void StartAction( UObject* OuterController ) { };
	//Action为外部调用函数
	virtual void Hover(UObject* OuterController) { };
	//Action为外部调用函数
	virtual void TouchLeave(UObject* OuterController) { };

public:
	FName CoCategory = FEventComponentCategoryUtil::ComponentBase;

protected:

};