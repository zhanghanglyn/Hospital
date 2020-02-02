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
	/* �����ϲ��Ĺ���ģ�� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USkeletalMesh*> MeshesToMerge;
	// �����ںϲ���������ĹǼܡ�
	// ������ɵĹǼ������������ա�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeleton* OuterSkeleton;
	/* ������ͼ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf< UAnimInstance > AnimClass;
	// �ȸ��¹Ǽ��ٺϲ������򣬺ϲ�����¡�
   // �������ṩ�Ǽܡ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bSkeletonBefore : 1;

protected:
	/* ΪActor����һ��SkeletonMeshComponent */
	void CreateMeshComponent();
	/* ���ݲ�����ʼ����ģ�� */
	UFUNCTION(BlueprintCallable)
	void CreateMesh();


	/* ���Դ��� ֱ�Ӳ���һ���������� */
	UFUNCTION(BlueprintCallable)
	void TestPlayAnimation();

protected:
	USkeletalMeshComponent*  PlayerSMC;	/* ��ɫ��ģ����� */
	USkeletalMesh* MainMesh;			/* �������Ĺ�����ɫ */


};
