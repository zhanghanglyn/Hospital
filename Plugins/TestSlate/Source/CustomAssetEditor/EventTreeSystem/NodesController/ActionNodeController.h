#pragma once
#include "CoreMinimal.h"
#include "NodeControllerBase.h"
#include "ActionNodeController.generated.h"

UCLASS()
class UActionNodeController : public UNodeControllerBase
{
	GENERATED_BODY()

public:

	UActionNodeController(const FObjectInitializer& ObjectInitializer) : UNodeControllerBase(ObjectInitializer) {
		ControllerCategory = FControllerCategoryUtil::ControllerAction;
	};

	virtual void Init(UObject* DynamicParam) override;

	virtual void Clear() override;

	virtual void ClearNextPage() override;
protected:
	UPROPERTY()
	UEventComponentBase* FreshObject;

};