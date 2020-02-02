#include "ActionNodeController.h"
#include "ActionNodes.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/ShowUIComponentBase.h"

#pragma optimize("",off)
/* 自动根据类型创建 */
void UActionNodeController::Init(UObject* DynamicParam)
{
	check(CurNode);

	UActionNodes* CurActionNode = Cast<UActionNodes>(CurNode);
	if (CurActionNode)
	{

		//根据类型创建并初始化组件
		if (CurActionNode->ActiveComponent && CurActionNode->ActiveOjb)//&& CurActionNode->ActiveComponent->IsValidLowLevelFast())
		{
			//应该判断下，如果此时该物体身上有这个组件，启用并设置初始值即可 全动态创建
			/*TArray<USceneComponent*> ComponentList = CurActionNode->ActiveOjb->GetRootComponent()->GetAttachChildren();
			for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
			{
				UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
				if (CurComponent->GetClass()->GetFName() == CurActionNode->ActiveComponent.Get()->GetFName())
				{
					CurComponent->SetActive(true);
					//根据Node上面的数据，设置Component
					CurComponent->SetData(CurActionNode->DataBase);
					CurComponent->OverDelegate.BindUObject(this, &UNodeControllerBase::ComponentsFinishCallBack);
					return;
				}
			}*/

			//如果身上没有，重新创建
			FreshObject = NewObject<UEventComponentBase>(CurActionNode->ActiveOjb.Get() , CurActionNode->ActiveComponent.Get());
			FreshObject->OverDelegate.BindUObject(this, &UNodeControllerBase::ComponentsFinishCallBack);

			//注册组件
			FreshObject->RegisterComponent();
			FreshObject->AttachToComponent(CurActionNode->ActiveOjb.Get()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			//根据Node上面的数据，设置Component
			FreshObject->SetActive(true);
			//FreshObject->SetData(CurActionNode->DataBase);
			FreshObject->SetData(CurActionNode->SaveDataComponnet);

			ComponentNodeActor = CurActionNode->ActiveOjb.Get();

		}
	}
}


//把生成的Component清除
void UActionNodeController::Clear()
{
	if (ComponentNodeActor != nullptr)
	{
		TArray<USceneComponent*> ComponentList = ComponentNodeActor->GetRootComponent()->GetAttachChildren();
		for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
		{
			UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
			if (CurComponent && CurComponent == FreshObject)
			{
				FreshObject = nullptr;
				CurComponent->Clear();
				CurComponent->RemoveFromRoot();
				CurComponent->DestroyComponent();
			}
		}
	}

	Super::Clear();
}

void UActionNodeController::ClearNextPage()
{
	if (ComponentNodeActor != nullptr)
	{
		TArray<USceneComponent*> ComponentList = ComponentNodeActor->GetRootComponent()->GetAttachChildren();
		for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
		{
			UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
			if (CurComponent && CurComponent == FreshObject)
			{
				FreshObject = nullptr;
				CurComponent->Clear();
				CurComponent->RemoveFromRoot();
				CurComponent->DestroyComponent();
			}
		}
	}

	Super::Clear();
}

#pragma optimize("",on)