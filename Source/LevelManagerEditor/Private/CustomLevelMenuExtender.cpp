// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomLevelMenuExtender.h"
#include "PersistentLevelSettings.h"

#include "Engine/LevelStreamingAlwaysLoaded.h"
#include "Engine/LevelStreamingDynamic.h"

#define LOCTEXT_NAMESPACE "CustomLevelMenuExtender"

void FCustomLevelMenuExtender::ExtendAssetsMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
{
	if (SelectedAssets.Num() == 1 && SelectedAssets[0].GetClass()->IsChildOf(UWorld::StaticClass()))
	{
		MenuBuilder.AddMenuEntry(
			FText::FromString("Register level"),
			FText::FromString("Register level in easy level manager."),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateStatic(&FCustomLevelMenuExtender::RegisterLevel, SelectedAssets[0]))
		);

		MenuBuilder.AddMenuEntry(
			FText::FromString("Set as the first display level"),
			FText::FromString("Set this level as the first display level in easy level manager."),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateStatic(&FCustomLevelMenuExtender::MarkAsFirst, SelectedAssets[0]))
		);
	}
}

void FCustomLevelMenuExtender::RegisterLevel(FAssetData SelectedAsset)
{
	UPersistentLevelSettings* Settings = GetMutableDefault<UPersistentLevelSettings>();
	if (!Settings)
	{
		return;
	}

	if (UWorld* World = Cast<UWorld>(SelectedAsset.GetAsset()))
	{
		TSoftObjectPtr<UWorld> SoftWorldPtr(World);

		// add self
		FMainLevelConfig MainLevelConfig;
		MainLevelConfig.SubLevels.Add(FSubLevelConfig(SoftWorldPtr, false));

		//UE_LOG(LogTemp, Warning, TEXT("Selected Level: %s"), *World->GetName());

		// add sub levels
		for (const auto& LevelStreaming : World->GetStreamingLevels())
		{
			if (!LevelStreaming)
			{
				break;
			}

			FSubLevelConfig Config;
			Config.Level = LevelStreaming->GetWorldAsset();

			if (LevelStreaming)
			{
				if (LevelStreaming->IsA<ULevelStreamingAlwaysLoaded>())
				{
					Config.bVisibleByBlueprint = false;
				}
				else if (LevelStreaming->IsA<ULevelStreamingDynamic>())
				{
					Config.bVisibleByBlueprint = true;
				}
				else
				{
					Config.bVisibleByBlueprint = false;
				}

				MainLevelConfig.SubLevels.Add(Config);
			}
		}

		// register in settings
		Settings->LevelConfig.Add(SoftWorldPtr, MainLevelConfig);
		Settings->SaveConfig();
	}
}

void FCustomLevelMenuExtender::MarkAsFirst(FAssetData SelectedAsset)
{
	UPersistentLevelSettings* Settings = GetMutableDefault<UPersistentLevelSettings>();
	if (!Settings)
	{
		return;
	}

	if (UWorld* World = Cast<UWorld>(SelectedAsset.GetAsset()))
	{
		TSoftObjectPtr<UWorld> SoftWorldPtr(World);

		if (Settings->LevelConfig.Find(SoftWorldPtr))
		{
			TMap<TSoftObjectPtr<UWorld>, FMainLevelConfig>& NewConfig = Settings->LevelConfig;
			for (auto& Level : NewConfig)
			{
				if (Level.Key.GetAssetName() == SoftWorldPtr.GetAssetName())
				{
					Level.Value.bVisibleAfterAllLevelLoad = true;
				}
				else
				{
					Level.Value.bVisibleAfterAllLevelLoad = false;
				}
			}
			Settings->LevelConfig = NewConfig;
			Settings->SaveConfig();
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("MissingLevel", "Please register level firstly."));
		}
	}
}
#undef LOCTEXT_NAMESPACE
