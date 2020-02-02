#pragma once
#include "CoreMinimal.h"
#include "NodeControllerBase.h"
#include "FunctionNodeController.generated.h"

/*
装配节点控制器，会在Actor上生成组件，如果之前已经装配过一样的组件，则激活，不添加
函数节点控制器，会调用一个设定好的函数，执行完即删除
*/
UCLASS()
class UFunctionNodeController : public UNodeControllerBase
{
	GENERATED_BODY()

public:

	UFunctionNodeController(const FObjectInitializer& ObjectInitializer) : UNodeControllerBase(ObjectInitializer) {
		ControllerCategory = FControllerCategoryUtil::ControllerAssemble;
	};

	virtual void Init(UObject* DynamicParam) override;

	virtual void Clear() override;

	virtual void ClearNextPage() override;

protected:
	UPROPERTY()
		TSoftObjectPtr<class UEventTreeMgr> EventTreeMgr;
};