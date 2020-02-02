#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ScenarioNodeNormal.h"
#include "Runtime/SlateCore/Public/Styling/SlateColor.h"
#include "EndNodes.generated.h"

UCLASS()
class UEndNodes : public UScenarioNodeNormal
{
	GENERATED_BODY()

public:

	UEndNodes();

	//~ Begin UEdGraphNode Interface
	//创建一个新节点的时候调用，会在里面生成该节点的Instance

	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End

	// 父类方法，继承后可以在里面添加新节点 
	virtual void AllocateDefaultPins() override;

	virtual UEdGraphPin* GetInPutPin() override;

};

//End应该是一个特殊的基类节点，他没有输出，只有输入！
/* 继承SBorder的Node显示类 */
class SScenarioEndNode : public SScenarioNodeNormal
{
public:
	SLATE_BEGIN_ARGS(SScenarioEndNode)
		: _NodeBgColor(FLinearColor::Gray)
	{
	}
	SLATE_ATTRIBUTE(FSlateColor, NodeBgColor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	/*更新Node已匹配Data更新*/
	virtual void UpdateGraphNode();

	/*创建Pin UI*/
	virtual void CreatePinWidgets();

	/* 把一个Pin添加到Node上，必须是新创建的Pin */
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd);
protected:
	class UEndNodes* OwnerGraphNode;

	//slate显示相关参数
protected:
	//TAttribute<FSlateColor> NodeBgColor;
};

