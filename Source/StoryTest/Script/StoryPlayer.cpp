// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryPlayer.h"

// Sets default values
AStoryPlayer::AStoryPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AStoryPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AStoryPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStoryPlayer::SetScenarioByPathName(FString PathName)
{
	FString path = FPaths::ProjectContentDir();

	//先清空当前剧本
	int32 Count = m_curData.Num();
	if (Count > int32(0))
		m_curData.Empty();

	//两种方法读取配置
	FString csvFilePath = ScenarioPath + PathName;
	//if (FPaths::FileExists((path + csvFilePath)))
	/*UDataTable *pDataTable = LoadObject<UDataTable>(NULL, *csvFilePath);
	if (pDataTable != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("yeah"));

	}*/

	//取一下scenarioConfig
	FString scenarioConfigPath = ScenarioPath + ScenarioConfigName;
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
	UDataTable *scenarioConfig = AssetLoader.LoadSynchronous<UDataTable>(FSoftObjectPath(scenarioConfigPath));
	if (scenarioConfig != nullptr)
	{
		FScenarioScenarioConfig* tmpPtr = nullptr;
		for (auto it : scenarioConfig->GetRowMap())
		{
			tmpPtr = (FScenarioScenarioConfig*)(it.Value);
			//如果是需要播放的剧本
			if (tmpPtr->scenarioId.Equals(PathName))
			{
				m_sequencePath = tmpPtr->se_path;
				break;
			}	
		}
	}


	//两种方法读取配置
	//FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
	UDataTable *TestData2 = AssetLoader.LoadSynchronous<UDataTable>(FSoftObjectPath(csvFilePath));
	if (TestData2 != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("yeah22"));
		FScenarioDataBase* tmpPtr = nullptr;
		for (auto it : TestData2->GetRowMap())
		{
			tmpPtr = (FScenarioDataBase*)(it.Value);
			//UE_LOG(LogTemp, Warning, TEXT("--- row:%s, key:%d, name:%d"), *it.Key.ToString(), tmpPtr->mId, tmpPtr->nextPage);
			m_curData.Add(tmpPtr->mId, *tmpPtr);
		}
	}

}

//正式开始跑剧情
void AStoryPlayer::StartRun()
{
	int32 Count = m_curData.Num();
	if (Count <= int32(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("ScenarioData is null!!!!"));
		return;
	}

	//做一些初始化的工作
	m_curPage = 1;

	GetScenarioSequencer(m_sequencePath);

	//PlayNext();
}

FScenarioDataBase AStoryPlayer::PlayNext()
{
	FScenarioDataBase *cur_page = m_curData.Find(m_curPage);

	return *cur_page;
}

void AStoryPlayer::GetScenarioSequencer(FString FullPath)
{
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
	IntroMovieSequence = AssetLoader.LoadSynchronous<ULevelSequence>( FSoftObjectPath(FullPath)); //<ULevelSequence>(FSoftObjectPath(FullPath));

	m_MovieActor = GetWorld()->SpawnActor<ALevelSequenceActor>();

	m_MovieActor->SetSequence(IntroMovieSequence);
	m_sequencePlayer = m_MovieActor->GetSequencePlayer();//SequencePlayer->Play();

	//MovieActor->GetSequence();
}



/*
ULevelSequence* LevelSequenceAsset = Cast<ULevelSequence>(MyLevelSequenceActor->LevelSequence.ResolveObject());
*/



