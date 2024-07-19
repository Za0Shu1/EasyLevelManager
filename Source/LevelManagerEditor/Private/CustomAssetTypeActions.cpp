// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAssetTypeActions.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FCustomAssetTypeActions::GetName() const
{
	return INVTEXT("Custom Asset");
}

FColor FCustomAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FCustomAssetTypeActions::GetSupportedClass() const
{
	return UWorld::StaticClass();
}

void FCustomAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	auto ObjectAssets = GetTypedWeakObjectPtrs<UObject>(InObjects);
	
	if (ObjectAssets.Num() == 1)
	{

		MenuBuilder.AddMenuEntry(
			LOCTEXT("InLable", "Label here."),
			LOCTEXT("InToolTip",
			        "ToolTip here"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &FCustomAssetTypeActions::DoSomeThing, ObjectAssets[0]),
			          FCanExecuteAction())
		);
	}
}

uint32 FCustomAssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}

void FCustomAssetTypeActions::DoSomeThing(TWeakObjectPtr<UObject> Object)
{
	
}

#undef LOCTEXT_NAMESPACE
