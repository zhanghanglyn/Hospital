// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryGameInstance.h"

void UStoryGameInstance::Init() 
{
	UMGManagerInstance = NewObject<UUMGManager>(); //UUMGManager::GetInstance();
	PlayerSystem = NewObject<UStoryPlaySystem>();

	if (GEngine)
	{
		UGameUserSettings* settings = GEngine->GetGameUserSettings();
		settings->SetScreenResolution(FIntPoint(1366, 768));
		settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		settings->SetVSyncEnabled(true);
	}

}

void UStoryGameInstance::OnStart()
{
	if (UMGManagerInstance != nullptr)
		UMGManagerInstance->CreateInstanceRootWidget(this);
}


UUMGManager * UStoryGameInstance::GetUMGManager()
{
	if(UMGManagerInstance != nullptr)
		return UMGManagerInstance;
	else
		UMGManagerInstance = NewObject<UUMGManager>();

	return UMGManagerInstance;
}

UStoryGameInstance* UStoryGameInstance::GetInstance(UWorld* _world)
{
	if (_world->GetGameInstance() != nullptr)
	{
		return Cast<UStoryGameInstance>( _world->GetGameInstance());
	}
	return nullptr;

}

UStoryGameInstance* UStoryGameInstance::GetInstance()
{
	if (GEngine)
	{
		UWorld *world = (GEngine->GetWorldContexts())[0].World();
		if (world)
		{
			UGameInstance *ins = world->GetGameInstance();
			return Cast<UStoryGameInstance>(ins);
		}
			
	}
	return nullptr;

}

void UStoryGameInstance::ChangeLevel(UWorld* _world, FName _levelName)
{
	UGameplayStatics::OpenLevel(_world, _levelName, false);
}

UWorld* UStoryGameInstance::InsGetWorld()
{
	UWorld *world = (GEngine->GetWorldContexts())[0].World();
	if (world)
	{
		return world;
	}

	return nullptr;
}