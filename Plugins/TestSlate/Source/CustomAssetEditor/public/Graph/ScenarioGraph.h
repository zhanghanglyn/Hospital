#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "ScenarioGraph.generated.h"

class FScenarioEditor;

UCLASS()
class UScenarioGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:
	//外部调用，设置根节点
	void SetRootNode( class URootNodes* InRootNode );

public:
	//UScenarioGraph(const FObjectInitializer& ObjectInitializer);

	FScenarioEditor* ScenarioEditor;

	UPROPERTY()
	class URootNodes* RootNode;
};