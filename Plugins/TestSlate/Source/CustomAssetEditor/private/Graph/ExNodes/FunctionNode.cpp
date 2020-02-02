#include "FunctionNode.h"
#include "BlueprintReflectionFunctions.h"
#include "UObject/UObjectHash.h"


//const FName UFunctionNode::ComponentPropertyName = "ActiveComponent";

UFunctionNode::UFunctionNode()
{
	NodeName = "Function";
	bOpenGraph = false;
	NodeCategory = FScenarioNodeUtil::NodeCategoryFunction;

	CurFunctionType = M_FunctionType;
}

TSharedPtr<SGraphNode> UFunctionNode::CreateVisualWidget()
{
	SAssignNew(SNodeWidgetShared, SScenarioNodeNormal, this).NodeBgColor(FLinearColor::FromSRGBColor(FColor(219, 221, 175)))
		.CategoryTEXT(FText::FromString(L"函数节点"));

	CurFunctionType = M_FunctionType;

	return SNodeWidgetShared;
}

/*
	如果更新的是选择的组件，则会重新生成DataBase数据,确保类型对应！
*/
#pragma optimize("",off)
void UFunctionNode::OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (M_FunctionType != CurFunctionType)
	{
		//如果是设置视角
		if (M_FunctionType == FFunctionType::CreateVewInfoData)
		{
			//以下屏蔽的设置全部是失败的，暂时先屏蔽唉
			/*if (UClass* DataClass = UBlueprintReflectionFunctions::GetClassByName(TestName))//UFunctionNode::GetEnumValString(M_FunctionType)))
			{
				SaveData = NewObject<UObject>(this, DataClass->StaticClass(), TEXT("FunctionNodeSaveData"));
			}

			//测试反射函数进行
			TArray<UObject*> result;

			UFunctionLibraryBase* aaa = NewObject< UFunctionLibraryBase>(this);

			GetObjectsOfClass(UFunctionLibraryBase::StaticClass(), result);
			for (TArray<UObject*>::TConstIterator iter = result.CreateConstIterator();iter ;++iter )
			{
				UObject* aa = (*iter);
				int32 a = 1;
			}*/
			if (LibraryObj)
			{
				FString FunctionName = GetEnumValString(M_FunctionType);
				UClass* uclass = LibraryObj.Get();
				UFunction* func = GetFunctionInLibrary(*FunctionName);//LibraryObj.Get()->FindFunctionByName(*FunctionName);
				if (func)
				{
					struct MyClass_Func_Parms   //定义一个结构用来包装参数和返回值，就像在gen.cpp里那样
					{
						//float param1;
						UObject* param1;
						UObject* ReturnValue;
					};
					MyClass_Func_Parms params;
					//params.param1 = 1;
					params.param1 = this;
					uclass->ProcessEvent(func, &params);

					SaveData = params.ReturnValue;
				}
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("这个函数节点啥也没做，选了个默认的"));
	}
	
	CurFunctionType = M_FunctionType;

	(SNodeWidgetShared.Get())->UpdateNodeNmae(NodeName);
}
#pragma optimize("",on)
FString UFunctionNode::GetEnumValString(FFunctionType EnumToGet)
{
	//UEnum* EnumPtr = Cast<UEnum>(EnumToGet);
	//FindObject 创建一个该枚举类型的对象，如果找到该枚举类型的对象，则不创建
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("FFunctionType"), true);
	FString CurShooterDataStr(EnumPtr->GetNameByValue((int)EnumToGet).ToString());//转化相应枚举字符串

	FString ClassName;

	CurShooterDataStr.Split("::",nullptr, &ClassName);

	return ClassName;
}

UFunction* UFunctionNode::GetFunctionInLibrary(FName FuncitonName)
{
	UClass* uclass = LibraryObj.Get();
	UFunction* func = LibraryObj.Get()->FindFunctionByName(FuncitonName);

	return func;
}