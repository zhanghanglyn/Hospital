// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomComponent.h"

FPrimitiveSceneProxy* UCustomComponent::CreateSceneProxy()
{
	return new FCustomPrimitiveSceneProxy(this);
	//return nullptr;
}
