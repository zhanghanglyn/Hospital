#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "SGraphNode.h"

//GraphNodeFactory 
struct FScenarioNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* InNode) const override;
};

//PinFactory
struct FScenarioPinFactory : public FGraphPanelPinFactory
{
public:
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* Pin) const override;
};

//Ò»¸öConnectFactory
struct FScenarioConnectionFactory : public FGraphPanelPinConnectionFactory
{
public:
	virtual class FConnectionDrawingPolicy* CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID,
		int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect,
		class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};
