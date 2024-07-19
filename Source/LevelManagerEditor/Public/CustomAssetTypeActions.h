// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetTypeActions_Base.h"

/**
 * 
 */

// this class need to add "UnrealEd" Module
// make sure supported class will be your own custom asset
class FCustomAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FCustomAssetTypeActions()
	{
	}

	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual uint32 GetCategories() override;
	// End of IAssetTypeActions interface

private:
	EAssetTypeCategories::Type MyAssetCategory;
	void DoSomeThing(TWeakObjectPtr<UObject> Object);
};
