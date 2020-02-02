#include "FunctionNodeController.h"
#include "FunctionNode.h"
#include "CustomAssetEditor/EventTreeSystem/EventTreeMgr.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"
#include "CustomAssetEditor/EventTreeSystem/FunctionLibrary/FunctionLibraryBase.h"

#pragma optimize("",off)
/* 自动根据类型创建 */
void UFunctionNodeController::Init(UObject* DynamicParam)
{
	check(CurNode);

	//将EventTreeMgr传入并且保存
	EventTreeMgr = Cast<UEventTreeMgr>(DynamicParam);

	UFunctionNode* CurFunctionNode = Cast<UFunctionNode>(CurNode);
	if (CurFunctionNode)
	{
			UClass* uclass = CurFunctionNode->LibraryObj.Get();
			if (ULibraryInfoDataBase* SaveData = Cast<ULibraryInfoDataBase>(CurFunctionNode->SaveData))
			{
				if (UFunction* func = CurFunctionNode->GetFunctionInLibrary(*(SaveData->FunctionName)))
				{
					FLibraryParamBase Param = SaveData->GetFunctionParam();
					Param.CharacterPlayer = EventTreeMgr.Get();
					uclass->ProcessEvent(func, &(Param));
				}
			}

	}
	
	ComponentsFinishCallBack(FName(""));
}


//把生成的Component清除
void UFunctionNodeController::Clear()
{

	Super::Clear();
}

//页面完成时并不会清除控件
void UFunctionNodeController::ClearNextPage()
{
	Super::ClearNextPage();
}

#pragma optimize("",on)