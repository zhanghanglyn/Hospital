#include "ScenarioFactories.h"
#include "ScenarioPin.h"
#include "ScenarioGraphSchema.h"
#include "ScenarioNodeNormal.h"
#include "EndNodes.h"
#include "ScenarioConnectionDrawingPolicy.h"


TSharedPtr<class SGraphNode> FScenarioNodeFactory::CreateNode(class UEdGraphNode* InNode) const
{
	if ( auto CurNode = Cast< UScenarioNodeNormal >(InNode) )
		return SNew(SScenarioNodeNormal, CurNode);
	else if(auto EndNode = Cast< UEndNodes >(InNode))
		return SNew(SScenarioEndNode, EndNode);

	return nullptr;
}

TSharedPtr<class SGraphPin> FScenarioPinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	if (Pin->PinType.PinCategory == "PC_Normal")
		return SNew(SScenarioPin, Pin);
	
	return nullptr;
}

FConnectionDrawingPolicy * FScenarioConnectionFactory::CreateConnectionPolicy(const UEdGraphSchema * Schema, int32 InBackLayerID,
	int32 InFrontLayerID, float ZoomFactor, const FSlateRect & InClippingRect, FSlateWindowElementList & InDrawElements, UEdGraph * InGraphObj) const
{
	if (Schema->IsA(UScenarioGraphSchema::StaticClass())) {
		return new FScenarioConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}
	return nullptr;

}
