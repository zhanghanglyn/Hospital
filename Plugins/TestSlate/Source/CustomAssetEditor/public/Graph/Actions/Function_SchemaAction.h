#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEd/Public/ScopedTransaction.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraph.h"
#include "CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphSchema.h"
#include "EditTabsUtil.h"
#include "Function_SchemaAction.generated.h"

USTRUCT()
struct FFunctionSchemaAction : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	FFunctionSchemaAction() : FEdGraphSchemaAction() {}
	FFunctionSchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping , UEdGraphNode* NodeTemplate , FName CategoryUtil)
		: FEdGraphSchemaAction(InNodeCategory , InMenuDesc , InToolTip , InGrouping)
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