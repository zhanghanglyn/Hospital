#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
//#include "ProjectN_Demo/EventTree/FunctionNodeLibrary/IFunctionLibraryHead.h"
#include "FunctionLibraryBase.generated.h"

/*
	张航 设计思路：
	在模块中进行函数的声明，但是实现函数放在GAMEPLAY中，测试：所有该文件中声明都使用class，
	在cpp中再包含对应的函数。这样做到模块化

	该函数只是一个模板，在GamePlay中去继承，在Node中就可以通过SubClassOf去设置了。测试下

*/
UCLASS()
class CUSTOMASSETEDITOR_API UFunctionLibraryBase : public UObject
{
	GENERATED_BODY()

public:

	UFunctionLibraryBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {};

public:
};
