#include "MyCustomAssetFactory.h"
#include "MyCustomAsset.h"

UMyCustomAssetFactory::UMyCustomAssetFactory()
{
	// Provide the factory with information about how to handle our asset
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UMyCustomAsset::StaticClass();

}

UObject* UMyCustomAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UMyCustomAsset* MyCustomAsset = NewObject<UMyCustomAsset>(InParent, Class, Name, Flags);
	return MyCustomAsset;
}