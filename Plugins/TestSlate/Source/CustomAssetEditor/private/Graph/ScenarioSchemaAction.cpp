#include "ScenarioSchemaAction.h"
#include "ScenarioNodeNormal.h"

#define LOCTEXT_NAMESPACE "ScenarioSchemaAction"

/* 在当个Action中只暂时写单引脚连接过来的 */
UEdGraphNode* FScenarioSchemaAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /* = true */)
{
	UEdGraphNode* ResultNode = nullptr;

#if WITH_EDITOR
	// If there is a template, we actually use it
	//if (NodeTemplate != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}
		//ResultNode = CreateNode(ParentGraph, FromPin, Location, NodeTemplate);
	}
#endif

	ResultNode = CreateNode(ParentGraph, FromPin, Location, NodeTemplate, bSelectNewNode);

	return ResultNode;
}

/*默认先暂时认为Template为null*/
UEdGraphNode* FScenarioSchemaAction::CreateNode(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, class UEdGraphNode* InNodeTemplate , bool bSelectNewNode)
{
	UEdGraphNode* ResultNode;
	if (InNodeTemplate == nullptr)
	{
		//根据不同类型创建
		/*{
			if(NodeCategoryUtil == FScenarioNodeUtil::NodeCategoryEnd)
			{
				FGraphNodeCreator<UScenarioNodeNormal> NodeCreater(*ParentGraph);
				ResultNode = NodeCreater.CreateNode(bSelectNewNode);
				ResultNode->SetFlags(RF_Transactional);
				NodeCreater.Finalize();
			}
			else if (NodeCategoryUtil == FScenarioNodeUtil::NodeCategoryNormal)
			{
				FGraphNodeCreator<UEndNodes> NodeCreater(*ParentGraph);
				ResultNode = NodeCreater.CreateNode(bSelectNewNode);
				ResultNode->SetFlags(RF_Transactional);
				NodeCreater.Finalize();
			}
			else
			{
				FGraphNodeCreator<UScenarioNodeNormal> NodeCreater(*ParentGraph);
				ResultNode = NodeCreater.CreateNode(bSelectNewNode);
				ResultNode->SetFlags(RF_Transactional);
				NodeCreater.Finalize();
			}
		}*/
		FGraphNodeCreator<UScenarioNodeNormal> NodeCreater(*ParentGraph);
		ResultNode = NodeCreater.CreateNode(bSelectNewNode);
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