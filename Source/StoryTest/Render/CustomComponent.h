// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/Public/PrimitiveSceneProxy.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "StoryTest/Render/CustomPrimitiveSceneProxy.h"
#include "CustomComponent.generated.h"


UCLASS(meta = (BlueprintSpawnableComponent))
class UCustomComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

};

