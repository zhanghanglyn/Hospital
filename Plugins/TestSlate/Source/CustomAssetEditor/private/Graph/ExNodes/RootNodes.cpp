#include "RootNodes.h"

URootNodes::URootNodes()
{
	NodeName = "Root";
	bOpenGraph = false;
	NodeCategory = FScenarioNodeUtil::NodeCategoryRoot;
}

void URootNodes::AllocateDefaultPins()
{
	//CreatePin(EEdGraphPinDirection::EGPD_Input,FName("TestPinIN") ,TEXT("IN"));
	CreatePin(EEdGraphPinDirection::EGPD_Output, FScenarioPinUtil::PinCategoryMulti, TEXT("OUT"));
}

TSharedPtr<SGraphNode> URootNodes::CreateVisualWidget()
{
	SAssignNew(SNodeWidgetShared, SScenarioNodeNormal, this).NodeBgColor(FLinearColor::Green)
		.CategoryTEXT(FText::FromString("")).BUseCategoryTitle(false);

	return SNodeWidgetShared;//SAssignNew(SNodeWidgetShared,SScenarioNodeNormal,this);
}

UEdGraphPin* URootNodes::GetOutPutPin()
{
	return Pins[0];
}