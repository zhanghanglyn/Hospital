#pragma once
#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UMyCustomAsset;

/**
 * 自定义一个资产编辑器接口
 */
class ICustomAssetEditor : public FAssetEditorToolkit
{

public:
	virtual UMyCustomAsset* GetCustomAsset() = 0;

	virtual void SetCustomAsset(UMyCustomAsset* InCustomAsset) = 0;

};