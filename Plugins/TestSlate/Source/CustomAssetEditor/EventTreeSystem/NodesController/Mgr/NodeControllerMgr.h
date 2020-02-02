#pragma once
#include "CoreMinimal.h"
#include "CustomAssetEditor/EventTreeSystem/NodesController/NodeControllerBase.h"
#include "EditTabsUtil.h"
#include "ScenarioNodeNormal.h"

class FNodeControllerMgr
{

public:
	FNodeControllerMgr(){};

	/* 根据类型创建一个节点控制器 */
	static UNodeControllerBase* CreateController(UScenarioNodeNormal* Node);

protected:

};