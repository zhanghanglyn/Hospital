
#include "SLayoutDetail.h"

#define  LOCTEXT_NAMESPACE "SLayoutDetail"

void SLayoutDetail::Construct(const FArguments& InArgs)
{
	this->ChildSlot
	[
        SNew(SDetail).Clicked(this , &SLayoutDetail::CreateArrowTest)
	];
}

//������canvas�д�����ͷ
void SLayoutDetail::CreateArrowTest()
{
	UE_LOG(LogTemp, Warning, TEXT("ddddddddd"));

}

#undef   LOCTEXT_NAMESPACE