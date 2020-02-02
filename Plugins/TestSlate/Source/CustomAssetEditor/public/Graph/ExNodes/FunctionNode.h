#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ScenarioNodeNormal.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/EventComponentBase.h"
#include "CustomAssetEditor/EventTreeSystem/ActiveComponent/Mgr/ActiveComponentMgr.h"
#include "CustomAssetEditor/EventTreeSystem/FunctionLibrary/FunctionLibraryBase.h"
#include "CustomAssetEditor/EventTreeSystem/FunctionLibrary/LibraryInfoDataBase.h"
#include "FunctionNode.generated.h"

/*
该枚举的变量名，就是在Library中调用的函数的函数名，所以要保证两边一一对应
*/
UENUM(BlueprintType)
enum class FFunctionType : uint8
{
	EM_Defualt				UMETA(DisplayName = "啥也不做，默认的"),
	CreateVewInfoData       UMETA(DisplayName = "设置视角"),
	
};

/*
	函数节点比较特殊，应该是直接调用一些单独的函数去完成一些单独的效果，初步设计是使用反射的方式
	该节点不存储调用函数的OBJ，只单纯的根据选择类型来初始化参数，最终根据参数进行调用
*/
UCLASS()
class UFunctionNode : public UScenarioNodeNormal
{
	GENERATED_BODY()

public:

	UFunctionNode();

	//~ Begin UEdGraphNode Interface
	//创建一个新节点的时候调用，会在里面生成该节点的Instance
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

	virtual void OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End

	/* 获取Enum 的 String的变量名 */
	static FString GetEnumValString(FFunctionType EnumToGet);

	/* 从TSubclassOf的LibraryBase中去取对应名称的函数 */
	UFunction* GetFunctionInLibrary(FName FuncitonName);
public:

	UPROPERTY()
	UObject* SaveData;

	UPROPERTY(EditAnywhere, Category = "调用函数")
	FFunctionType M_FunctionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "必选，选FunctionLibrary")
	TSubclassOf<UFunctionLibraryBase> LibraryObj;

protected:
	FFunctionType CurFunctionType;

	//UPROPERTY()
	//class UFunctionLibrary* tttt;


};


