#include "EventComponentBase.h"

void UEventComponentBase::FinishCallBack() {
	//��������Ϊ������
	//SetActive(false,true);
	//Deactivate();
	
	if(OverDelegate.IsBound())
		OverDelegate.Execute(FName(""));

	Clear();

	//this->RemoveFromRoot();
	//this->DestroyComponent();
};
