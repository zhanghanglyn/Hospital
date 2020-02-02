// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Engine/SkeletalMesh.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Public/SkeletalMeshMerge.h"
#include "SkeletalMeshMerge.h"
#include "ModuleActor.generated.h"

UCLASS()
class STORYTEST_API AModuleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModuleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/* 用来合并的骨骼模型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USkeletalMesh*> MeshesToMerge;
	// 将用于合并后网格体的骨架。
	// 如果生成的骨架正常，则留空。
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeleton* OuterSkeleton;
	/* 动画蓝图 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf< UAnimInstance > AnimClass;
	// 先更新骨架再合并。否则，合并后更新。
   // 还必须提供骨架。
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bSkeletonBefore : 1;

protected:
	/* 为Actor创建一个SkeletonMeshComponent */
	void CreateMeshComponent();
	/* 根据参数开始创建模型 */
	UFUNCTION(BlueprintCallable)
	void CreateMesh();


	/* 测试代码 直接播放一个动画测试 */
	UFUNCTION(BlueprintCallable)
	void TestPlayAnimation();

protected:
	USkeletalMeshComponent*  PlayerSMC;	/* 角色的模型组件 */
	USkeletalMesh* MainMesh;			/* 创建出的骨骼角色 */


};
