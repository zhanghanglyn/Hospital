// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleActor.h"

// Sets default values
AModuleActor::AModuleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AModuleActor::BeginPlay()
{
	Super::BeginPlay();

	CreateMeshComponent();
}

// Called every frame
void AModuleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma optimize("" , off)
void AModuleActor::CreateMeshComponent()
{
	if (PlayerSMC != nullptr)
		return;
	PlayerSMC = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), "PlayerMesh");
	if (PlayerSMC != nullptr)
	{
		PlayerSMC->RegisterComponent();
		PlayerSMC->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}
#pragma optimize("" , on)

void AModuleActor::CreateMesh()
{
	if (PlayerSMC == nullptr)
		CreateMeshComponent();

	MainMesh = NewObject<USkeletalMesh>();

	/* 先更新骨骼 */
	if (bSkeletonBefore && OuterSkeleton != nullptr)
		MainMesh->Skeleton = this->OuterSkeleton;

	TArray<FSkelMeshMergeSectionMapping> InForceSectionMapping;
	FSkeletalMeshMerge MeshMerge(MainMesh, MeshesToMerge, InForceSectionMapping, 0);
	if (MeshMerge.DoMerge())
	{
		//MeshMerge.MergeSkeleton();
		PlayerSMC->SetSkeletalMesh(MainMesh);
	}

	/* 后更新骨骼 */
	if (!bSkeletonBefore && OuterSkeleton != nullptr)
		MainMesh->Skeleton = this->OuterSkeleton;
}

void AModuleActor::TestPlayAnimation()
{
	PlayerSMC->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	PlayerSMC->SetAnimClass(AnimClass);
}