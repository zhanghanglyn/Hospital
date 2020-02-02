// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryPlayerPawn.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"
#include "CustomAssetEditor/EventTreeSystem/EventTreeTestObject.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Mgr/ActiveComponentMgr.h"

// Sets default values
AStoryPlayerPawn::AStoryPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//初始化一个UI
	/*if (m_SelectUI == NULL)
	{
		if (UClass* SelevtWidgetNew = LoadClass<UUserWidget>(NULL, TEXT("/Game/StoryTest/BP_SelectWidgetNew")))
		{
			if (GetWorld() != NULL)
			{
				if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
				{
					m_SelectUI = CreateWidget<USelectWidget>(PC, SelevtWidgetNew);
					check(m_SelectUI);
					m_SelectUI->AddToViewport();
					//m_SelectUI->SetVisibility(ESlateVisibility::Hidden);

					//绑定选项委托回调
					m_SelectUI->m_clickOneDelegate.BindUObject(this, &AStoryPlayerPawn::SelectOneCall);
				}
			}
		}
	}*/

}

// Called when the game starts or when spawned
void AStoryPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//初始化一个UI
	if (m_SelectUI == NULL)
	{
		UClass* TempSelectUI = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/StoryTest/UI/BP_SelectWidgetNew.BP_SelectWidgetNew_C'"));
		if (TempSelectUI != nullptr)
		{
			m_SelectUI = CreateWidget<USelectWidget>(GetWorld()->GetFirstPlayerController(), TempSelectUI);
			check(m_SelectUI);
			m_SelectUI->AddToViewport();
			m_SelectUI->SetVisibility(ESlateVisibility::Hidden);
			SetInputModeGameOnly();

			//绑定选项委托回调
			m_SelectUI->m_clickOneDelegate.BindUObject(this, &AStoryPlayerPawn::SelectOneCall);
		}
	}


	//测试测试！！！！！！10.10.11
	/*WidgetComponent = NewObject<UWidgetComponent>(RootComponent, UWidgetComponent::StaticClass());
	if (WidgetComponent != nullptr)
	{
		WidgetComponent->RegisterComponent();
		WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		//"WidgetBlueprint'/Game/StoryTest/UI/TESTUI.TESTUI_C'"

		//UClass* Widget = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/StoryTest/UI/StoryControl.StoryControl_C'"));
		UClass* Widget = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/StoryTest/UI/TESTUI.TESTUI_C'"));

		if (Widget != nullptr)
		{
			WidgetComponent->SetWidgetClass(Widget);
			//UUserWidget *temp_widget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), Widget);
			//temp_widget->AddToViewport();
		}
	}*/

	//TestFunc();
}

// Called every frame 
void AStoryPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStoryPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AStoryPlayerPawn::CheckActiveObjAction);
	//绑定视角移动
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AStoryPlayerPawn::TurnRate)
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

void AStoryPlayerPawn::SetInputModeGameOnly()
{
	FInputModeGameOnly Mode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(Mode);
}

void AStoryPlayerPawn::SetInputModeUIOnly()
{
	FInputModeUIOnly Mode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(Mode);
}

/************************************************************************/
/*                    操作相关				                            */
/************************************************************************/
void AStoryPlayerPawn::Click()
{
	//会判断是在普通的点击屏幕，或是在进行事件
	UE_LOG(LogTemp, Warning, TEXT("i'm click!!!!"));
	m_clickDelegate.ExecuteIfBound();
}

void AStoryPlayerPawn::TurnRate(float Rate)
{
	//会判断是在普通的点击屏幕，或是在进行事件
	//UE_LOG(LogTemp, Warning, TEXT("Trun rate!!!! %d"), Rate);
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

///////////////////////////////////////////////

void AStoryPlayerPawn::SetScenarioByPathName(FString PathName)
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

void AStoryPlayerPawn::InitializePage()
{
	bSequencerOver = false;
	bEventOver = false;
	bSelectOver = false;

	if (m_SelectUI != nullptr)
		m_SelectUI->SetVisibility(ESlateVisibility::Hidden);
	SetInputModeGameOnly();
}

//正式开始跑剧情
void AStoryPlayerPawn::StartRun()
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

void AStoryPlayerPawn::PlayNext()
{
	//如果大于最大页数或者为-1，播放器结束
	if (m_curPage > m_curData.Num() || m_curPage == -1)
		return ;

	FScenarioDataBase *cur_page = m_curData.Find(m_curPage);
	InitializePage();

	//播放剧情
	PlaySequence(*cur_page);

	//播放事件
	PlayEvent(cur_page->eventId);

	//播放选项
	PlaySelection(*cur_page);

	//return *cur_page;
}

//每一页的每一个动作完成之后都会统一调用一次该回调函数，如果该页动作都执行完，直接进行下一页
void AStoryPlayerPawn::CallBackByEveryPageAction( int32 _page)
{
	UE_LOG(LogTemp, Warning, TEXT("page click call : CallBackByEveryPageAction"));

	if (bSequencerOver == true && bEventOver == true)
	{
		//19.09.24 这里应该用一个处理当前页点击以及当前页数据等的函数来处理page 之后再playNext
		if (_page != -1)
			m_curPage = _page;
		else
		{
			//如果大于最大页数或者为-1，播放器结束 暂时的垃圾代码
			if (m_curPage > m_curData.Num() || m_curPage == -1)
					return;
			else
				m_curPage = m_curData.Find(m_curPage)->nextPage;
		}
			
		PlayNext();
	}	
	else
		UE_LOG(LogTemp, Warning ,TEXT("this page is not over!!") );
}


/************************************************************************/
/*                        播放sequence相关                              */
/************************************************************************/

void AStoryPlayerPawn::GetScenarioSequencer(FString FullPath)
{
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
	IntroMovieSequence = AssetLoader.LoadSynchronous<ULevelSequence>(FSoftObjectPath(FullPath)); //<ULevelSequence>(FSoftObjectPath(FullPath));

	m_MovieActor = GetWorld()->SpawnActor<ALevelSequenceActor>();

	m_MovieActor->SetSequence(IntroMovieSequence);
	m_sequencePlayer = m_MovieActor->GetSequencePlayer();//SequencePlayer->Play();

	//MovieActor->GetSequence();
}

//播放动画
void AStoryPlayerPawn::PlaySequence(FScenarioDataBase cur_page)
{
	//先判断如果不需要播放SE,设置播放完成标识并且停止播放
	if (cur_page.second_temp == -1)
	{
		if (m_sequencePlayer != NULL)
		{
			if (m_sequencePlayer->IsPlaying() == true)
				m_sequencePlayer->Stop();
		}
		bSequencerOver = true;
	}
	else
	{
		if (m_sequencePlayer != NULL)
		{
			if (m_sequencePlayer->IsPlaying() == true)
				m_sequencePlayer->Stop();

			m_sequencePlayer->Play();
			bSequencerOver = false;
		}
	}
	//设置点击函数为普通点击
	m_clickDelegate.Unbind();
	m_clickDelegate.BindUObject(this, &AStoryPlayerPawn::SequencerClick);

	if (finishDelegate.IsBound() == false)
	{
		finishDelegate.BindUFunction(this, "SequencePlayOverCallBack");
		m_sequencePlayer->OnFinished.AddUnique(finishDelegate);
	}
	
	//设置不使用摄像机剪辑！
	SetBeControlCamera(cur_page.bOutCameraControl);
}

void AStoryPlayerPawn::SequencerClick()
{
	CallBackByEveryPageAction();
}

//Sequencer播放完毕回调
void AStoryPlayerPawn::SequencePlayOverCallBack()
{
	bSequencerOver = true;
	UE_LOG(LogTemp, Warning, TEXT("now !!!!!!!!!!!!SequencePlayOverCallBack!!!!!!!!!!!"));
}

void AStoryPlayerPawn::SetBeControlCamera(bool _bControl)
{
	if (m_sequencePlayer != NULL)
		m_sequencePlayer->SetDisableCameraCuts(_bControl);
}



/************************************************************************/
/*                       处理事件										*/
/************************************************************************/                

//根据ID去处理事件 //暂时就写死一个事件处理方式
void AStoryPlayerPawn::PlayEvent(FString _eventId)
{
	//
	if (_eventId.IsEmpty())
		bEventOver = true;
	else   //为不同的函数做不同的绑定以及设定参数
	{
		//如果有事件，绑定委托，如果没有
		if (_eventId == "EVENT_PICK")
		{
			m_clickDelegate.Unbind();
			m_clickDelegate.BindUObject(this, &AStoryPlayerPawn::PickEventClick);
		}
	}
}

//捡起事件,之后的物体会只配置为单独的可点击物体
void AStoryPlayerPawn::PickEventClick()
{
	//通过射线查找是否是需要选中的物体
	//找到摄像机开始发射射线位置
	//this->GetActorForwardVector();
	APlayerCameraManager* tempCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector startPos = tempCameraManager->GetCameraLocation();
	FVector forward = tempCameraManager->GetActorForwardVector();
	FVector endPos = startPos + forward * 100000;

	TArray<FHitResult> temp_HitResult;
	//添加射线参数
	FCollisionQueryParams cqq(FName(TEXT("Combattrace")), true, NULL);
	cqq.bTraceComplex = true;
	cqq.bReturnPhysicalMaterial = false;
	cqq.AddIgnoredActor(this);

	GetWorld()->LineTraceMultiByObjectType(temp_HitResult, startPos, endPos, ECC_PhysicsBody , cqq);
	DrawDebugLine(this->GetWorld(), startPos, endPos, FColor::Red, true, 15000.0f);

	//如果有撞到东西
	if (temp_HitResult.Num() > 0)
	{
		for (int i = 0; i< temp_HitResult.Num() ; i++)
		{
			FString hitName = temp_HitResult[i].GetActor()->GetName();
			int32 tempPageCount = DealDataByClickName(hitName);
			if (tempPageCount != -1)
			{
				UE_LOG(LogTemp, Warning, TEXT("Now i hit %s !"), *hitName);
				CallBackByEveryPageAction(tempPageCount);
				break;
			}
		}
	}
}

int32 AStoryPlayerPawn::DealDataByClickName(FString _hitName)
{
	if (_hitName.Equals("PickOne"))
	{
		bEventOver = true;
		return 1;
	}
	else if (_hitName.Equals("PickOne2"))
	{
		bEventOver = true;
		return 3;
	}
	else if (_hitName.Equals("PickOne3"))
	{
		bEventOver = true;
		return 4;
	}

	return -1;
}

/************************************************************************/
/*                       处理选项										*/
/************************************************************************/
void AStoryPlayerPawn::PlaySelection(FScenarioDataBase cur_page)
{
	if (cur_page.select.IsEmpty() == true && cur_page.selectToPage.IsEmpty() == true)
	{
		bSelectOver = true;
		return;
	}

	//把选项以及页数拆分出来
	FString select = cur_page.select;
	TArray<FString> temp_select;
	select.ParseIntoArray(temp_select, TEXT("#"), true);

	FString select_page = cur_page.selectToPage;
	TArray<FString> temp_to_page;
	select_page.ParseIntoArray(temp_to_page, TEXT("#"), true);
	TArray<int32> int_to_page;
	for (int i = 0; i < temp_to_page.Num(); i++)
	{
		int_to_page.Add(FCString::Atoi(*temp_to_page[i]));
	}

	//解绑之前的点击函数
	m_clickDelegate.Unbind();

	//从蓝图中加载选项UI 之后可以提出 显示UI
	if (m_SelectUI != NULL)
	{
		SetInputModeUIOnly();
		m_SelectUI->SetVisibility(ESlateVisibility::Visible);
		m_SelectUI->SetCurPageData(temp_select, int_to_page);
	}
}

//选项的三个点击回调
void AStoryPlayerPawn::SelectOneCall(SelectParam param)
{
	UE_LOG(LogTemp, Warning, TEXT("Now i select next page : %d !"), param.pageId);
	bSelectOver = true;
	CallBackByEveryPageAction(param.pageId);

}


/************************************************************************/
/*                        处理选项over		                            */
/************************************************************************/

void AStoryPlayerPawn::TestFunc()
{
	/*UUIWidgetComponentBase *temp_WidgetComponent = NewObject<UUIWidgetComponentBase>(RootComponent, UUIWidgetComponentBase::StaticClass());
	if (temp_WidgetComponent != nullptr)
	{
		temp_WidgetComponent->RegisterComponent();
		temp_WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		//"WidgetBlueprint'/Game/StoryTest/UI/TESTUI.TESTUI_C'"

		//UClass* Widget = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/StoryTest/UI/StoryControl.StoryControl_C'"));
		UClass* Widget = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/StoryTest/UI/TESTUI.TESTUI_C'"));

		if (Widget != nullptr)
		{
			temp_WidgetComponent->SetUI(Widget);
		}
	}*/

	WidgetComponent = UUIWidgetComponentBase::CreateUIWidgetBase(RootComponent, true,true);
	//WidgetComponent->SetUI("WidgetBlueprint'/Game/StoryTest/UI/TouchUITest.TouchUITest_C'");
	UClass* Widget = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/StoryTest/UI/StoryControl.StoryControl_C'"));
	UUserWidget* tempaa = WidgetComponent->SetUI(Widget);
	UUserWidgetBase *tempcc = Cast < UUserWidgetBase>(tempaa);
	UTestTouchUI *tempbb = Cast < UTestTouchUI>(tempcc);
	if (tempbb != NULL)
		FString aa = tempbb->fs;

	//该方法有效！
	//UTestTouchUI* touchUI = WidgetComponent->SetUI<UTestTouchUI>("WidgetBlueprint'/Game/StoryTest/UI/TouchUITest.TouchUITest_C'");
	WidgetComponent->SetUI<UUserWidget>("WidgetBlueprint'/Game/StoryTest/UI/StoryControl.StoryControl_C'");
	//if (touchUI != NULL)
		//FString aa = touchUI->fs;
	WidgetComponent->SetUIPosition( FVector(0,50,230) );
	WidgetComponent->SetUIRotation(FRotator(0,0,190));

	//在静态物品上添加,成功
	TArray<AActor*> _tArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), _tArray);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), _tArray);
	AStaticMeshActor* tempMesh = NULL;
	//ACharacter *tempMesh = nullptr;
	if (_tArray.Num() > 0)
	{
		for (int i=0;i < _tArray.Num() ; i++)
		{
			if (UKismetSystemLibrary::GetDisplayName(_tArray[i]) == "TestThing")
			//if (UKismetSystemLibrary::GetDisplayName(_tArray[i]) == "TestCharactor")
			{
				tempMesh = Cast<AStaticMeshActor>(_tArray[i]);
				//tempMesh = Cast<ACharacter>(_tArray[i]);
				break;
			}
		}
	}

	if (tempMesh != NULL)
	{
		tempMesh->SetActorTickEnabled(true);
		UUIWidgetComponentBase *temp_base = UUIWidgetComponentBase::CreateUIWidgetBase(tempMesh->GetRootComponent(), true,true);
		//UUIWidgetComponentBase *temp_base = UUIWidgetComponentBase::CreateUIWidgetBase(tempMesh->GetRootComponent(), true, true);
		temp_touchUI = temp_base->SetUI<UTestTouchUI>("WidgetBlueprint'/Game/StoryTest/UI/TouchUITest.TouchUITest_C'");
	}


	//全屏UMG测试
	FString screenPath = "WidgetBlueprint'/Game/StoryTest/UI/BF_FullScreenTest.BF_FullScreenTest_C'";
	//UFullScreenTest *test1 = Cast<UFullScreenTest>(UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->CreateScreenWidget(screenPath,GetWorld(), UFullScreenTest::StaticClass(), "Test1"));
	//UFullScreenTest *test1 = UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->CreateScreenWidget<UFullScreenTest>(screenPath, GetWorld(), UFullScreenTest::StaticClass(), "Test1" ,10);
	//if (test1 != nullptr)
		//test1->TestFunc();

	FString screenPath2 = "WidgetBlueprint'/Game/StoryTest/UI/BP_FullScreenTest2.BP_FullScreenTest2_C'";
	//UFullScreenWidgetBase *testUI2 = UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->CreateScreenWidget<UFullScreenWidgetBase>(screenPath2, GetWorld(), UFullScreenWidgetBase::StaticClass(), "Test2");

	UStoryGameInstance* instance = UStoryGameInstance::GetInstance(GetWorld());
	/*UFullScreenWidgetBase *temp_Test1 = instance->GetUMGManager()->GetScreenWidget("Test1");
	UFullScreenTest *test2 = nullptr;
	if (temp_Test1 != nullptr)
		test2 = Cast< UFullScreenTest>(temp_Test1);
	if (test2 != nullptr)
		test2->TestFunc();
	*/
	//测试在此把UI添加到不会被清除的部分！
	/*UE_LOG(LogTemp, Warning, TEXT("i'm create!!!!!!!!!"));
	UClass* Temp_Widget = LoadClass<UFullScreenWidgetBase>(NULL, screenPath2.GetCharArray().GetData());
	if (Temp_Widget != nullptr)
	{
		UFullScreenWidgetBase *NewWidget = CreateWidget<UFullScreenWidgetBase>(GetWorld(), Temp_Widget);
		if (NewWidget != nullptr)
		{
			//instance->GetGameViewportClient()->AddViewportWidgetContent(NewWidget->TakeWidget());
			NewWidget->AddToViewport();
		}
	}*/

	FInputModeUIOnly uiOnly;
	//UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->SetInputMode(GetWorld(), uiOnly);
	this->SetInputModeUIOnly();

	//UFullScreenTest *test3 = UUMGManager::GetInstance()->GetScreenWidget<UFullScreenTest>("Test1");
	//if (test3 != nullptr)
	//	test3->TestFunc();
}

void AStoryPlayerPawn::TestFuncTwo()
{

	this->SetInputModeUIOnly();
	if (WidgetComponent != NULL)
	{
		UTestTouchUI* touchUI = WidgetComponent->SetUI<UTestTouchUI>("WidgetBlueprint'/Game/StoryTest/UI/TouchUITest.TouchUITest_C'");
		//if (touchUI!=NULL)
		//{
		///	touchUI
		//}
	}

	//if (temp_touchUI != nullptr)
	//{
	//	temp_touchUI->SetPlay();
	//}

	//return;


	UStoryGameInstance* instance = UStoryGameInstance::GetInstance(GetWorld());
	//instance->GetUMGManager()->ClearAll();
	//instance->GetUMGManager()->ClearInsUMG();

	//UStoryGameInstance::GetInstance(GetWorld())->ChangeLevel( GetWorld(), FName("/Game/StoryTest/Level/StoryTestLevel"));

	UFullScreenWidgetBase* test111 = instance->GetUMGManager()->GetInsUMGByUID(testUID);
	//UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->DeleteInsUMGWidget(test111);
	UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->DeleteInsUMGWidget(testUID);


	return;
	
	//UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->DeleteInsUMGByName("Test1");
	//UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->ClearInsUMG();
	

	FString screenPath = "WidgetBlueprint'/Game/StoryTest/UI/BF_FullScreenTest.BF_FullScreenTest_C'";
	UFullScreenWidgetBase *NewWidget = instance->GetUMGManager()->CreateInstanceWidget(GetWorld(), screenPath);
	if (NewWidget != nullptr)
		NewWidget->SetZorder<UCanvasPanelSlot>(1);
}

void AStoryPlayerPawn::TestFuncThree()
{
	/*UStoryGameInstance* instance2 = UStoryGameInstance::GetInstance();

	FString screenPath2 = "WidgetBlueprint'/Game/StoryTest/UI/BP_FullScreenTest2.BP_FullScreenTest2_C'";
	UStoryGameInstance* instance = UStoryGameInstance::GetInstance(GetWorld());


	UE_LOG(LogTemp, Warning, TEXT("i'm create!!!!!!!!!"));
	UFullScreenWidgetBase *NewWidget = instance->GetUMGManager()->CreateInstanceWidget(GetWorld(), screenPath2);
	testUID = NewWidget->GetUID();

	//UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->DeleteInsUMGWidget(NewWidget);
	//UStoryGameInstance::GetInstance(GetWorld())->GetUMGManager()->DeleteInsUMGWidget(NewWidget->GetUID());

	//测试判断类对象中是否有某个函数
	NewWidget->SetPosition<	UCanvasPanelSlot>(FVector2D(100, 200));
	if (NewWidget != nullptr)
	{
		NewWidget->SetZorder<UCanvasPanelSlot>(2);
		//NewWidget->SetAnchors<UCanvasPanelSlot>(FAnchors(1, 1, 1, 1));
	}
	*/

	//storyPlayer测试
	UStoryGameInstance* instance = UStoryGameInstance::GetInstance(GetWorld());
	UStoryPlayerBase* base = NewObject<UStoryPlayerBase>();
	USequencerPlayer* sequancerPlayer = NewObject<USequencerPlayer>();
	
	base->RegistToSystem(instance->PlayerSystem);
	sequancerPlayer->Init(GetWorld());
	sequancerPlayer->RegistToSystem(instance->PlayerSystem);

	instance->PlayerSystem->Play();
	
	
}
#pragma optimize("",off)
void AStoryPlayerPawn::CheckActiveObjAction()
{
	APlayerCameraManager* tempCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector startPos = tempCameraManager->GetCameraLocation();
	FVector forward = tempCameraManager->GetActorForwardVector();
	FVector endPos = startPos + forward * 100000;


	TArray<FHitResult> temp_HitResult;
	//添加射线参数
	FCollisionQueryParams cqq(FName(TEXT("Combattrace")), true, NULL);
	cqq.bTraceComplex = true;
	cqq.bReturnPhysicalMaterial = false;
	cqq.AddIgnoredActor(this);

	GetWorld()->LineTraceMultiByObjectType(temp_HitResult, startPos, endPos, ECC_PhysicsBody, cqq);
	DrawDebugLine(this->GetWorld(), startPos, endPos, FColor::Red, true, 15000.0f);

	//如果有撞到东西
	if (temp_HitResult.Num() > 0)
	{
		for (int i = 0; i < temp_HitResult.Num(); i++)
		{
			AActor* TempActor = temp_HitResult[i].GetActor();
			TArray<USceneComponent*> ComponentList = TempActor->GetRootComponent()->GetAttachChildren();
			//TArray<UActorComponent*> ComponentList = TempActor->GetComponentsByClass(UEventComponentBase::StaticClass());
			for (TArray<USceneComponent*>::TConstIterator iter = ComponentList.CreateConstIterator(); iter; ++iter)
			{
				//AEventTreeTestObject* aa = NewObject< AEventTreeTestObject>();
				UEventComponentBase* CurComponent = Cast<UEventComponentBase>(*iter);
				if (CurComponent && CurComponent->IsActive())
				{
					//FActiveComponentMgr::RunAction(CurComponent);
					CurComponent->StartAction();
				}
			}

		}
	}
}
#pragma optimize("",on)