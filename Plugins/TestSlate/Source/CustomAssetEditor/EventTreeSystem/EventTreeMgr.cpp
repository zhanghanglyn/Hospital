#include "EventTreeMgr.h"
#include "MyCustomAsset.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphNode.h"
//#include "BlueprintReflectionFunctions.h"
#include "RootNodes.h"
#include "EndNodes.h"
#include "ScenarioGraph.h"

bool UEventTreeMgr::GetTreeAsset(FString Path)
{
	UObject* TreeAssetTemp = LoadObject<UObject>(NULL, *Path);
	if (TreeAssetTemp)
	{
		TreeAsset = Cast<UMyCustomAsset>(TreeAssetTemp);
		if(TreeAsset)
		{
			TreeGraph = Cast<UScenarioGraph>(TreeAsset->StoryGraph);
			if (TreeGraph)
			{
				RootNode = TreeGraph->RootNode;
				return true;
			}
		}		
	}	
	return false;
}

//从Array中查找所有的链接节点，并且为其生成控制器，将控制器注册到本次节点事件中
void UEventTreeMgr::CreateController(TArray<UEdGraphNode *> LinkNodes)
{
	//NodeControllers.Empty();
	//Clear();

	for (TArray<UEdGraphNode *>::TConstIterator Iter = LinkNodes.CreateConstIterator(); Iter; ++Iter)
	{
		UScenarioNodeNormal* GraphNode = Cast<UScenarioNodeNormal>(*Iter);

		UNodeControllerBase* TempController = FNodeControllerMgr::CreateController(GraphNode);
		//最好是把装配节点的控制器与正常节点的分开，不然真的不好处理
		//if (TempController->ControllerCategory == FControllerCategoryUtil::ControllerAction )
			NodeControllers.Add(TempController);
		//else if(TempController->ControllerCategory == FControllerCategoryUtil::ControllerAssemble)
		//	AssembleNodeControllers.Add(TempController);
		//绑定播放完毕回调
		TempController->OverDelegate.BindUObject(this, &UEventTreeMgr::ControllerOverCallBack);
		TempController->InitController(GraphNode , this);

	}
}

//检测是否是End节点！暂时认为，只有一个End
bool UEventTreeMgr::CheckEndNodes(TArray<class UEdGraphNode *> LinkNodes)
{
	for (TArray<UEdGraphNode *>::TConstIterator Iter = LinkNodes.CreateConstIterator(); Iter; ++Iter)
	{
		//如果是End节点，就结束了
		if (UEndNodes* EndNode = Cast<UEndNodes>(*Iter))
		{
			return true;
		}
	}

	return false;
}

/*
	startRun的流程应该是，为该节点装配节点控制器，通过判断控制器的完成状态来进行下一节点的切换
*/
#pragma optimize("",off)
void UEventTreeMgr::StartRun(UObject* OverCallObj, FName _UFunctionName)
{
	//绑定一个播放完毕的委托
	if (EventPlayOverCallDelegate.IsBound())
		EventPlayOverCallDelegate.Unbind();
	EventPlayOverCallDelegate.BindUFunction(OverCallObj, _UFunctionName);

	TArray<UEdGraphNode*> LinkNodes = RootNode->GetAllOutNodeLinked();

	//获取节点类型，根据节点类型分配Controller
	if (LinkNodes.Num() > 0)
		CreateController(LinkNodes);

}

/* 收到本次节点已完成后，获取当前节点的接下去的节点，重复环节。
*/

void UEventTreeMgr::PlayNext()
{
	TArray<UEdGraphNode*> LinkNodes = CurNode->GetAllOutNodeLinked();

	//判断是否是结束节点
	if (CheckEndNodes(LinkNodes))
	{
		Finish();
		return;
	}
		

	if (LinkNodes.Num() > 0)
		CreateController(LinkNodes);
}

/* 收到一个Controller返回后，检测是否所有Controller状态均为已完成，如果不是则继续等待,其实一个节点应该只有一个Controller，
所以，true的时候就Break就好了   20.1.21 不一定！
*/
void UEventTreeMgr::ControllerOverCallBack(FName ControllerCategory)
{
	bool bFinishAll = false;

	for (TArray<UNodeControllerBase*>::TConstIterator iter = NodeControllers.CreateConstIterator();iter;++iter)
	{
		if ((*iter)->BControlOver == true)
		{
			bFinishAll = true;
			CurNode = (*iter)->CurNode;
			(*iter)->ClearNextPage();
			//break;
		}
		else
			bFinishAll = false;
	}
	/*for (TArray<UNodeControllerBase*>::TConstIterator iter = AssembleNodeControllers.CreateConstIterator(); iter; ++iter)
	{
		if ((*iter)->BControlOver == true)
		{
			bFinishAll = true;
			CurNode = (*iter)->CurNode;
			(*iter)->ClearNextPage();
			//break;
		}
		else
			bFinishAll = false;
	}*/

	NodeControllers.Empty();
	//AssembleNodeControllers.Empty();

	if (bFinishAll == true)
	{
		PlayNext();
	}
		
}
#pragma optimize("",on)

void UEventTreeMgr::Finish()
{
	UE_LOG(LogTemp, Warning, TEXT("本次事件播放结束~"));
	if (EventPlayOverCallDelegate.IsBound())
	{
		EventPlayOverCallDelegate.ExecuteIfBound(CurChoose);
	}
		
	Clear();
}
#pragma optimize("",off)
/*清空需要清空的数据等*/
void UEventTreeMgr::Clear()
{
	EventPlayOverCallDelegate.Unbind();

	//遍历所有Component然后清除
	for (TArray<UNodeControllerBase*>::TConstIterator iter = NodeControllers.CreateConstIterator(); iter; ++iter)
	{
		(*iter)->Clear();
	}
	//for (TArray<UNodeControllerBase*>::TConstIterator iter = AssembleNodeControllers.CreateConstIterator(); iter; ++iter)
	//{
	//	(*iter)->Clear();
	//}
		
	NodeControllers.Empty();
	//AssembleNodeControllers.Empty();
}
#pragma optimize("",on)

/************************************************************************/
/*                           与3C接口相关                               */
/************************************************************************/
void UEventTreeMgr::StartTouch(UObject* Controller3C, UObject* TouchObj, FKey key)
{
	AActor* TempActor = Cast<AActor>(TouchObj);
	TArray<USceneComponent*> ComponentList = TempActor->GetRootComponent()->GetAttachChildren();
	for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
	{
		UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
		if (CurComponent && CurComponent->IsActive())
		{
			CurComponent->StartAction(Controller3C);
		}
	}
}

void UEventTreeMgr::Hover(UObject* Controller3C, UObject* TouchObj)
{
	AActor* TempActor = Cast<AActor>(TouchObj);
	TArray<USceneComponent*> ComponentList = TempActor->GetRootComponent()->GetAttachChildren();
	for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
	{
		UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
		if (CurComponent && CurComponent->IsActive())
		{
			CurComponent->Hover(Controller3C);
		}
	}
}

void UEventTreeMgr::TouchLeave(UObject* Controller3C, UObject* TouchObj)
{
	AActor* TempActor = Cast<AActor>(TouchObj);
	TArray<USceneComponent*> ComponentList = TempActor->GetRootComponent()->GetAttachChildren();
	for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
	{
		UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
		if (CurComponent && CurComponent->IsActive())
		{
			CurComponent->TouchLeave(Controller3C);
		}
	}
}