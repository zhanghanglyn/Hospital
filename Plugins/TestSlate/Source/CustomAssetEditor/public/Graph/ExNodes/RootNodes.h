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
	//����һ���½ڵ��ʱ����ã������������ɸýڵ��Instance
	virtual bool CanUserDeleteNode() const override { return false; };
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End

	/* ���෽�����̳к��������������½ڵ� */
	virtual void AllocateDefaultPins();

	virtual UEdGraphPin* GetOutPutPin() override;

};