#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FLevelManagerEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterSettings();
    void UnregisterSettings();
    
    static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets);
    
    void AddLevelEditorMenuExtension(FMenuBuilder& Builder);
    void OnConfigLevel();
};
