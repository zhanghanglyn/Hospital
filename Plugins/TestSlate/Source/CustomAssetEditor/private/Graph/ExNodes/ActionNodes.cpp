#include "ActionNodes.h"


const FName UActionNodes::ComponentPropertyName = "ActiveComponent";

UActionNodes::UActionNodes()
{
	NodeName = "Action";
	bOpenGraph = false;
	NodeCategory = FScenarioNodeUtil::NodeCategoryAction;

	//if (DataBase == nullptr)
	//	DataBase = NewObject<UComponentNodeDataBase>();
	if (ActiveComponent)
		CurComponentName = ActiveComponent->GetFName();//FActiveComponentMgr::GetEventComponentCategory(ActiveComponent);
}

TSharedPtr<SGraphNode> UActionNodes::CreateVisualWidget()
{
	SAssignNew(SNodeWidgetShared, SScenarioNodeNormal, this).NodeBgColor(FLinearColor::FromSRGBColor(FColor(78, 151, 131)))
		.CategoryTEXT(FText::FromString(L"互动节点"));

	return SNodeWidgetShared;
}

/*
	如果更新的是选择的组件，则会重新生成DataBase数据,确保类型对应！
*/
void UActionNodes::OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent)
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
	//if (PropertyChangedEvent.GetPropertyName() == UActionNodes::ComponentPropertyName && !ChangeEventComponentName.IsEqual(CurComponentName))
	FName ChangeEventComponentName = ActiveComponent->GetFName();
	if (PropertyChangedEvent.GetPropertyName() == UActionNodes::ComponentPropertyName && !ChangeEventComponentName.IsEqual(CurComponentName))
	{
		//如果是ShowUI类型，则对应生成  
		//if (FActiveComponentMgr::GetEventComponentCategory(ActiveComponent) == FEventComponentCategoryUtil::ComponentShowUI)
		//{
			//DataBase = nullptr;
			//DataBase = FActiveComponentMgr::CreateComponentNodeData(ActiveComponent , this);
		//}
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