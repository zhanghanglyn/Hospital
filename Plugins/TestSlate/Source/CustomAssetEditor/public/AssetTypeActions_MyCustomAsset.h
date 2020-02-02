#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h"
#include "MyCustomAsset.h"

class FAssetTypeActions_MyCustomAsset : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	
	virtual uint32 GetCategories() override;

	static EAssetTypeCategories::Type AssetCategoryBit;
};