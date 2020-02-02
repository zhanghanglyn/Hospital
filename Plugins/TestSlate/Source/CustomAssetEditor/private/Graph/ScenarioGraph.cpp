#include "ScenarioGraph.h"
#include "RootNodes.h"
#include "ScenarioGraphSchema.h"

UScenarioGraph::UScenarioGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UScenarioGraphSchema::StaticClass();
}

void UScenarioGraph::SetRootNode(URootNodes* InRootNode)
{
	RootNode = InRootNode;
}