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
	static UStoryGameInstance* GetInstance();  //�÷���ȷʵ������
	virtual void Init();

	virtual void OnStart();

	//�л��ؿ�
	void ChangeLevel(UWorld* _world, FName _levelName);

	UWorld* InsGetWorld();


public:
	UFUNCTION(BlueprintCallable)
	UUMGManager* GetUMGManager();

public:
	//�ڴ˳�ʼ�����ұ���һ��UMGManager
	UUMGManager* UMGManagerInstance;
	UStoryPlaySystem* PlayerSystem;
	
};
