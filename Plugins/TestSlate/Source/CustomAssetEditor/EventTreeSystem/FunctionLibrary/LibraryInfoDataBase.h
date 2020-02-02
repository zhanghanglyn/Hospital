#pragma once

#include "CoreMinimal.h"
#include "LibraryInfoDataBase.generated.h"

/*传递给执行Function的ParamBase*/
USTRUCT()
struct CUSTOMASSETEDITOR_API FLibraryParamBase
{
	GENERATED_BODY()

public:
	FLibraryParamBase() {};

public:
	UPROPERTY()
	UObject* CharacterPlayer;
};

/*
	所有Library的Data的基类
*/
UCLASS()
class CUSTOMASSETEDITOR_API ULibraryInfoDataBase : public UObject
{
	GENERATED_BODY()

public:
	//UViewInfoData() {};
	ULibraryInfoDataBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {};

	UFUNCTION()
	virtual FLibraryParamBase GetFunctionParam()
	{
		FLibraryParamBase* param = new FLibraryParamBase();

		return *param;
	}


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "程序修改，请不要动")
	FString FunctionName = "ttttt";

};
