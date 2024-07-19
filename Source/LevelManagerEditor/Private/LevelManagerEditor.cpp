#include "LevelManagerEditor.h"

#include "ContentBrowserModule.h"
#include "CustomLevelMenuExtender.h"
#include "EditorLevelUtils.h"
#include "ISettingsModule.h"
#include "LevelEditor.h"
#include "PersistentLevelSettings.h"
#include "Engine/LevelStreamingDynamic.h"
#include "EditorAssetLibrary.h"

#define LOCTEXT_NAMESPACE "FLevelManagerEditorModule"

void FLevelManagerEditorModule::StartupModule()
{
	RegisterSettings();

	// extend content browser
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(
		"ContentBrowser");
	TArray<FContentBrowserMenuExtender_SelectedAssets>& ContentBrowserExtenderDelegates = ContentBrowserModule.
		GetAllAssetViewContextMenuExtenders();
	ContentBrowserExtenderDelegates.Add(
		FContentBrowserMenuExtender_SelectedAssets::CreateStatic(
			&FLevelManagerEditorModule::OnExtendContentBrowserAssetSelectionMenu));

	// extend level editor
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	{
		TSharedPtr<FExtender> Extender = MakeShareable(new FExtender);
		Extender->AddMenuExtension("LevelEditor",
		                           EExtensionHook::After,
		                           nullptr,
		                           FMenuExtensionDelegate::CreateRaw(
			                           this, &FLevelManagerEditorModule::AddLevelEditorMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(Extender);
	}
}

void FLevelManagerEditorModule::ShutdownModule()
{
	UnregisterSettings();
}

void FLevelManagerEditorModule::RegisterSettings()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings("Project", "Project", "EasyLevelManager",
		                                 LOCTEXT("EasyLevelManagerSettingsName", "Easy Level Manager"),
		                                 LOCTEXT("EasyLevelManagerSettingsDescription",
		                                         "Configure main level settings"),
		                                 GetMutableDefault<UPersistentLevelSettings>()
		);
	}
}

void FLevelManagerEditorModule::UnregisterSettings()
{
	// unregister settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Project", "EasyLevelManager");
	}
}

TSharedRef<FExtender> FLevelManagerEditorModule::OnExtendContentBrowserAssetSelectionMenu(
	const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> Extender = MakeShareable(new FExtender());
	Extender->AddMenuExtension(
		"GetAssetActions",
		EExtensionHook::First,
		nullptr,
		FMenuExtensionDelegate::CreateStatic(&FCustomLevelMenuExtender::ExtendAssetsMenu, SelectedAssets)
	);
	return Extender;
}

void FLevelManagerEditorModule::AddLevelEditorMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(
		FText::FromString("ConfigLevel"),
		FText::FromString("Config persistent level by easy level manager."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FLevelManagerEditorModule::OnConfigLevel))
	);
}

void FLevelManagerEditorModule::OnConfigLevel()
{
	if (UPersistentLevelSettings* Settings = GetMutableDefault<UPersistentLevelSettings>())
	{
		if (GEditor)
		{
			UWorld* PersistentLevel = GEditor->GetEditorWorldContext().World();
			TArray<FString> LevelPackageNames;
			for (auto Config : Settings->LevelConfig)
			{
				for (auto SubLevel : Config.Value.SubLevels)
				{
					LevelPackageNames.AddUnique(SubLevel.Level.GetLongPackageName());
				}
			}
			EditorLevelUtils::AddLevelsToWorld(PersistentLevel, MoveTemp(LevelPackageNames),
			                                   ULevelStreamingDynamic::StaticClass());
			UEditorAssetLibrary::SaveLoadedAsset(PersistentLevel);
		}
	}
}

IMPLEMENT_MODULE(FLevelManagerEditorModule, LevelManagerEditor)
#undef LOCTEXT_NAMESPACE
