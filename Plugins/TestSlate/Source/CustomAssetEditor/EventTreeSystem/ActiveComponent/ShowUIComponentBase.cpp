#include "ShowUIComponentBase.h"

void UShowUIComponentBase::StartAction(UObject* OuterController)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ShowName);
	FinishCallBack();
}

void UShowUIComponentBase::SetData(UEventComponentBase* Data)
{
	UShowUIComponentBase* ShouUIData = Cast<UShowUIComponentBase>(Data);
	if (ShouUIData)
	{
		ShowName = ShouUIData->ShowName;
	}

}