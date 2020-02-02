// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ScenarioDataBase.generated.h"

/**
 * 
 */
USTRUCT()
struct FScenarioDataBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FScenarioDataBase();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		int32 mId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		int32 nextPage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		int32 second_temp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString caption;
	//�׳��¼�ID
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString eventId;		
	//�Ƿ񲻿��ƾ�ͷ
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		bool bOutCameraControl;
	//ѡ���б�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString select;
	//ѡ����תҳ��
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")
		FString selectToPage;

	//�¼�����
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FTestData")

};
