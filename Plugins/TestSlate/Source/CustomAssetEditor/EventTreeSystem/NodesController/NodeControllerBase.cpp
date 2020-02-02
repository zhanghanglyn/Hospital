#include "NodeControllerBase.h"

void UNodeControllerBase::ControllerFinish()
{
	BControlOver = true;
	OverDelegate.ExecuteIfBound(ControllerCategory);
}

/* 根据Node上的数据进行Controller以及对应的Actor的初始化以及组件的赋予 */
/*void UNodeControllerBase::Init()
{

}*/

void UNodeControllerBase::ComponentsFinishCallBack(FName Paramm)
{
	ControllerFinish();
}

void UNodeControllerBase::Clear()
{
	CurNode = nullptr;
	ComponentBase = nullptr;
	ComponentNodeActor = nullptr;
}

void UNodeControllerBase::ClearNextPage()
{
	CurNode = nullptr;
	ComponentBase = nullptr;
	ComponentNodeActor = nullptr;
}