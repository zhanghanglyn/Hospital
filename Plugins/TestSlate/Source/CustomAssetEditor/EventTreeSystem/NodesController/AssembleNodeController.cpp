#include "AssembleNodeController.h"
#include "AssembleNodes.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"

#pragma optimize("",off)
/* 自动根据类型创建 */
void UAssembleNodeController::Init(UObject* DynamicParam)
{
	check(CurNode);

	UAssembleNodes* CurActionNode = Cast<UAssembleNodes>(CurNode);
	if (CurActionNode)
	{
		//根据类型创建并初始化组件
		if (CurActionNode->ActiveComponent && CurActionNode->ActiveOjb)//&& CurActionNode->ActiveComponent->IsValidLowLevelFast())
		{
			if (CurActionNode->M_AssembleType == AssembleType::EM_Assemble)
			{
				//应该判断下，如果此时该物体身上有这个组件，启用并设置初始值即可 全动态创建
				TArray<USceneComponent*> ComponentList = CurActionNode->ActiveOjb.Get()->GetRootComponent()->GetAttachChildren();
				for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
				{
					if (UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter))
					{
						if (CurComponent->GetClass()->GetFName() == CurActionNode->ActiveComponent.Get()->GetFName())
						{
							CurComponent->SetActive(true);
							//根据Node上面的数据，设置Component
							CurComponent->SetData(CurActionNode->SaveDataComponnet);
							//if (CurComponent->OverDelegate.IsBound())
							//	CurComponent->OverDelegate.Unbind();
							//CurComponent->OverDelegate.BindUObject(this, &UNodeControllerBase::ComponentsFinishCallBack);

							//ComponentNodeActor = CurActionNode->ActiveOjb.Get();
							ComponentsFinishCallBack(FName(""));
							return;
						}
					}
					
				}

				//如果身上没有，重新创建
				UEventComponentBase* FreshObject = NewObject<UEventComponentBase>(CurActionNode->ActiveOjb.Get(), CurActionNode->ActiveComponent.Get());
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
			else
			{
				TArray<USceneComponent*> ComponentList = CurActionNode->ActiveOjb.Get()->GetRootComponent()->GetAttachChildren();
				for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
				{
					if (UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter))
					{
						if (CurComponent->GetClass()->GetFName() == CurActionNode->ActiveComponent.Get()->GetFName())
						{
							CurComponent->Clear();
							CurComponent->RemoveFromRoot();
							CurComponent->DestroyComponent(true);
							CurComponent = nullptr;
							ComponentsFinishCallBack(FName(""));
							return;
						}
					}
				}
			}
			
		}
	}
	ComponentsFinishCallBack(FName(""));
}


//把生成的Component清除
void UAssembleNodeController::Clear()
{
	if (ComponentNodeActor != nullptr)
	{
		TArray<USceneComponent*> ComponentList = ComponentNodeActor->GetRootComponent()->GetAttachChildren();
		for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
		{
			UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
			if (CurComponent)
			{
				CurComponent->Clear();
				CurComponent->RemoveFromRoot();
				CurComponent->DestroyComponent();
			}
		}
	}

	Super::Clear();
}

//页面完成时并不会清除控件
void UAssembleNodeController::ClearNextPage()
{

}

#pragma optimize("",on)