#pragma once
#include "CoreMinimal.h"
#include "EventComponentBase.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Data/NodeDataShowUI.h"
#include "ShowUIComponentBase.generated.h"

/* 事件Component的基类 */
UCLASS(ClassGroup = (EventTreeComponent) , meta = (BlueprintSpawnableComponent))
class UShowUIComponentBase : public UEventComponentBase
{
	GENERATED_BODY()

public:
	UShowUIComponentBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {};

	virtual void StartAction(UObject* OuterController) override;

	//Action为外部调用函数
	virtual void Hover(UObject* OuterController) override { };

	//Action为外部调用函数
	virtual void TouchLeave(UObject* OuterController) override { };


	virtual void SetData(UEventComponentBase* Data) override;
public:
	UPROPERTY(EditAnywhere, Category = "测试测试")
	FString ShowName = "Test!!!!";
};
