#include "AssembleNodes.h"


const FName UAssembleNodes::ComponentPropertyName = "ActiveComponent";

UAssembleNodes::UAssembleNodes()
{
	NodeName = "Assemble";
	bOpenGraph = false;
	NodeCategory = FScenarioNodeUtil::NodeCategoryAssembly;

	//if (DataBase == nullptr)
	//	DataBase = NewObject<UComponentNodeDataBase>();
	if (ActiveComponent)
		CurComponentName = ActiveComponent->GetFName();//FActiveComponentMgr::GetEventComponentCategory(ActiveComponent);
}

//void UAssembleNodes::AllocateDefaultPins()
//{
//	CreatePin(EEdGraphPinDirection::EGPD_Input, FScenarioPinUtil::PinInputBreakSourceOutput, TEXT("In"));
//	CreatePin(EEdGraphPinDirection::EGPD_Output, FScenarioPinUtil::PinCategoryMulti, TEXT("Out"));
//}

TSharedPtr<SGraphNode> UAssembleNodes::CreateVisualWidget()
{
	SAssignNew(SNodeWidgetShared, SScenarioNodeNormal, this).NodeBgColor(FLinearColor::FromSRGBColor(FColor(160, 151, 197)))
		.CategoryTEXT(FText::FromString(L"装配节点"));

	return SNodeWidgetShared;
}

/*
	如果更新的是选择的组件，则会重新生成DataBase数据,确保类型对应！
*/
void UAssembleNodes::OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (ActiveComponent == nullptr)
	{
		if (SaveDataComponnet != nullptr)
		{
			SaveDataComponnet->RemoveFromRoot();
			SaveDataComponnet->DestroyComponent(true);
			SaveDataComponnet = nullptr;
		}
		return;
	}

	//FName ChangeEventComponentName = FActiveComponentMgr::GetEventComponentCategory(ActiveComponent);
	//if (PropertyChangedEvent.GetPropertyName() == UAssembleNodes::ComponentPropertyName && !ChangeEventComponentName.IsEqual(CurComponentName))
	FName ChangeEventComponentName = ActiveComponent->GetFName();
	if(PropertyChangedEvent.GetPropertyName() == UAssembleNodes::ComponentPropertyName && !ChangeEventComponentName.IsEqual(CurComponentName))
	{
		//20.1.19 修改为ActiveComponent创建实例用来保存
		if (SaveDataComponnet != nullptr)
		{
			SaveDataComponnet->RemoveFromRoot();
			SaveDataComponnet->DestroyComponent(true);
			SaveDataComponnet = nullptr;
		}
		SaveDataComponnet = NewObject<UEventComponentBase>(this, ActiveComponent.Get());
	}

	CurComponentName = ChangeEventComponentName;

	(SNodeWidgetShared.Get())->UpdateNodeNmae(NodeName);
}