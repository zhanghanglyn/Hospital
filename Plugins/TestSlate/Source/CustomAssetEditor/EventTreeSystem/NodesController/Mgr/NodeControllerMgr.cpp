#include "NodeControllerMgr.h"
#include "CustomAssetEditor/EventTreeSystem/NodesController/ActionNodeController.h"
#include "CustomAssetEditor/EventTreeSystem/NodesController/AssembleNodeController.h"
#include "CustomAssetEditor/EventTreeSystem/NodesController/FunctionNodeController.h"

UNodeControllerBase* FNodeControllerMgr::CreateController(UScenarioNodeNormal* Node)
{
	//如果是个Action节点
	if (Node->NodeCategory == FScenarioNodeUtil::NodeCategoryAction)
	{
		//test 先创建Base的controller
		UActionNodeController* NodeController = NewObject< UActionNodeController>();
		//NodeController->CurNode = Node;
		//NodeController->InitController(Node);

		return NodeController;
	}
	//如果是装配节点
	if (Node->NodeCategory == FScenarioNodeUtil::NodeCategoryAssembly)
	{
		//test 先创建Base的controller
		UAssembleNodeController* NodeController = NewObject< UAssembleNodeController>();
		//NodeController->CurNode = Node;
		//NodeController->InitController(Node);

		return NodeController;
	}
	//如果是函数节点
	if (Node->NodeCategory == FScenarioNodeUtil::NodeCategoryFunction)
	{
		//test 先创建Base的controller
		UFunctionNodeController* NodeController = NewObject< UFunctionNodeController>();
		//NodeController->CurNode = Node;
		//NodeController->InitController(Node);

		return NodeController;
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("None Control can be Create"));
	}

	return nullptr;
}