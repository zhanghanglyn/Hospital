#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEd/Public/ScopedTransaction.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraph.h"
#include "CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphSchema.h"
#include "EditTabsUtil.h"
#include "ScenarioSchemaAction.generated.h"

USTRUCT()
struct FScenarioSchemaAction : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	/** Template of node we want to create */
	UPROPERTY()
	class UEdGraphNode* NodeTemplate;

	FScenarioSchemaAction() : FEdGraphSchemaAction() {}
	FScenarioSchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping , UEdGraphNode* NodeTemplate , FName CategoryUtil)
		: FEdGraphSchemaAction(InNodeCategory , InMenuDesc , InToolTip , InGrouping) , NodeTemplate(nullptr)
	{
		NodeCategoryUtil = CategoryUtil;
	};

	//两种performAction,一种有引脚过来的，一种没有
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true ) override;
	//virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, TArray<UEdGraphPin *>& FromPins, const FVector2D Location, bool bSelectNewNode = true) override {};

	UEdGraphNode* CreateNode(UEdGraph* ParentGraph , UEdGraphPin* FromPin, const FVector2D Location, class UEdGraphNode* InNodeTemplate, bool bSelectNewNode);

protected:
	FName NodeCategoryUtil;
};