// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LEVELMANAGEREDITOR_API FCustomLevelMenuExtender
{
public:
	static void ExtendAssetsMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets);

private:
	static void RegisterLevel(FAssetData SelectedAsset);
	static void MarkAsFirst(FAssetData SelectedAsset);
	
};
