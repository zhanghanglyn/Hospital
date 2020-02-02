﻿#include "Assemble_SchemaAction.h"
#include "AssembleNodes.h"

#define LOCTEXT_NAMESPACE "ActionSchemaAction"

/* 在当个Action中只暂时写单引脚连接过来的 */
UEdGraphNode* FAssembleSchemaAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /* = true */)
{
	UEdGraphNode* ResultNode = nullptr;

#if WITH_EDITOR
	// If there is a template, we actually use it
	//if (NodeTemplate != nullptr)
	//{
		const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}
		//ResultNode = CreateNode(ParentGraph, FromPin, Location, NodeTemplate);
	//}
#endif

	ResultNode = CreateNode(ParentGraph, FromPin, Location, nullptr, bSelectNewNode);

	return ResultNode;
}

/*默认先暂时认为Template为null*/
UEdGraphNode* FAssembleSchemaAction::CreateNode(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, class UEdGraphNode* InNodeTemplate , bool bSelectNewNode)
{
	UEdGraphNode* ResultNode;
	if (InNodeTemplate == nullptr)
	{
		FGraphNodeCreator<UAssembleNodes> NodeCreater(*ParentGraph);
		ResultNode = NodeCreater.CreateNode(bSelectNewNode);
		//ResultNode = NodeCreater.CreateUserInvokedNode(bSelectNewNode);
		ResultNode->SetFlags(RF_Transactional);
		//ResultNode->Rename(*(FString("Scenario")), ParentGraph);
		NodeCreater.Finalize();
	}
	else
	{
		ResultNode = DuplicateObject<UEdGraphNode>(InNodeTemplate, ParentGraph);
	}
		
	ResultNode->AutowireNewNode(FromPin);

	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;

	ParentGraph->NotifyGraphChanged();

	return ResultNode;
}

#undef LOCTEXT_NAMESPACE