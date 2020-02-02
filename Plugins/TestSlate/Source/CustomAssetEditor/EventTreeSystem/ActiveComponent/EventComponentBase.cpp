#include "EventComponentBase.h"

void UEventComponentBase::FinishCallBack() {
	//把自身设为不激活
	//SetActive(false,true);
	//Deactivate();
	
	if(OverDelegate.IsBound())
		OverDelegate.Execute(FName(""));

	Clear();

	//this->RemoveFromRoot();
	//this->DestroyComponent();
};
