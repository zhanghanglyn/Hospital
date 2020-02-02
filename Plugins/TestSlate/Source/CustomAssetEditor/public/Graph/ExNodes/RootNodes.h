#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ScenarioNodeNormal.h"
#include "RootNodes.generated.h"

UCLASS()
class URootNodes : public UScenarioNodeNormal
{
	GENERATED_BODY()

public:

	URootNodes();

	//~ Begin UEdGraphNode Interface
	//创建一个新节点的时候调用，会在里面生成该节点的Instance
	virtual bool CanUserDeleteNode() const override { return false; };
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End

	/* 父类方法，继承后可以在里面添加新节点 */
	virtual void AllocateDefaultPins();

	virtual UEdGraphPin* GetOutPutPin() override;

};