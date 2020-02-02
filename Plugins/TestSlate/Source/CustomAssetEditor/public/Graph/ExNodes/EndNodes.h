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
	//����һ���½ڵ��ʱ����ã������������ɸýڵ��Instance

	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End

	// ���෽�����̳к��������������½ڵ� 
	virtual void AllocateDefaultPins() override;

	virtual UEdGraphPin* GetInPutPin() override;

};

//EndӦ����һ������Ļ���ڵ㣬��û�������ֻ�����룡
/* �̳�SBorder��Node��ʾ�� */
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

	/*����Node��ƥ��Data����*/
	virtual void UpdateGraphNode();

	/*����Pin UI*/
	virtual void CreatePinWidgets();

	/* ��һ��Pin��ӵ�Node�ϣ��������´�����Pin */
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd);
protected:
	class UEndNodes* OwnerGraphNode;

	//slate��ʾ��ز���
protected:
	//TAttribute<FSlateColor> NodeBgColor;
};

