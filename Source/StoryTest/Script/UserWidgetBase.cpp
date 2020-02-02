// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetBase.h"

UUserWidgetBase::UUserWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CreateUID();
}

void UUserWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_PanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
}


void UUserWidgetBase::SetPosition(FVector2D pos)
{
	if (m_PanelSlot == nullptr)
		m_PanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
	
	if (m_PanelSlot == nullptr)
		return;
	m_PanelSlot->SetPosition(pos);

}

//设置Zorder
void UUserWidgetBase::SetZorder(int32 zorder)
{
	if (m_PanelSlot == nullptr)
		m_PanelSlot = Cast<UCanvasPanelSlot>(this->Slot);

	if (m_PanelSlot == nullptr)
		return;
	m_PanelSlot->SetZOrder(zorder);
}

//设置锚点
void UUserWidgetBase::SetAnchors(FAnchors InAnchors)
{
	if (m_PanelSlot == nullptr)
		m_PanelSlot = Cast<UCanvasPanelSlot>(this->Slot);

	if (m_PanelSlot == nullptr)
		return;
	m_PanelSlot->SetAnchors(InAnchors);
}

//获取UID
FString UUserWidgetBase::GetUID()
{
	return UID;
}

//设置UID
void UUserWidgetBase::CreateUID()
{
	FDateTime Time = FDateTime::Now();
	int64 Timestamp = Time.ToUnixTimestamp();
	
	UID = (this->GetFName()).ToString() + FString::FromInt(Timestamp);
}