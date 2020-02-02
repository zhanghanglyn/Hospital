// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/GameFramework/GameUserSettings.h"
#include "Engine/Classes/Engine/World.h"
#include "StoryPlaySystem/StoryPlaySystem.h"
#include "FullUMG/UMGManager.h"
#include "StoryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STORYTEST_API UStoryGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	static UStoryGameInstance* GetInstance(UWorld* _world);
	static UStoryGameInstance* GetInstance();  //该方法确实有问题
	virtual void Init();

	virtual void OnStart();

	//切换关卡
	void ChangeLevel(UWorld* _world, FName _levelName);

	UWorld* InsGetWorld();


public:
	UFUNCTION(BlueprintCallable)
	UUMGManager* GetUMGManager();

public:
	//在此初始化并且保存一份UMGManager
	UUMGManager* UMGManagerInstance;
	UStoryPlaySystem* PlayerSystem;
	
};
