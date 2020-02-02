#pragma once
#include "CoreMinimal.h"
#include "NodeControllerBase.h"
#include "AssembleNodeController.generated.h"

/*
装配节点控制器，会在Actor上生成组件，如果之前已经装配过一样的组件，则激活，不添加
*/
UCLASS()
class UAssembleNodeController : public UNodeControllerBase
{
	GENERATED_BODY()

public:

	UAssembleNodeController(const FObjectInitializer& ObjectInitializer) : UNodeControllerBase(ObjectInitializer) {
		ControllerCategory = FControllerCategoryUtil::ControllerAssemble;
	};

	virtual void Init(UObject* DynamicParam) override;

	virtual void Clear() override;

	virtual void ClearNextPage() override;

protected:
	void AssembleOver();

};