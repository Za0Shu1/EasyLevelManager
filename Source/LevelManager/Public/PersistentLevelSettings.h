// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PersistentLevelSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSubLevelConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Easy Level Manager | SubLevel")
	TSoftObjectPtr<UWorld> Level;

	UPROPERTY(Config, EditAnywhere, Category = "Easy Level Manager | SubLevel")
	bool bVisibleByBlueprint = false;
};

USTRUCT(BlueprintType)
struct FMainLevelConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Easy Level Manager | Level")
	TArray<FSubLevelConfig> SubLevels;

	UPROPERTY(Config, EditAnywhere, Category = "Easy Level Manager | Level")
	bool bVisibleAfterAllLevelLoad = false;
};

UCLASS(Config = Engine, DefaultConfig)
class LEVELMANAGER_API UPersistentLevelSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Easy Level Manager | Config")
	TMap<TSoftObjectPtr<UWorld>, FMainLevelConfig> LevelConfig;

	TArray<TSoftObjectPtr<UWorld>> GetAllSubLevels();
};


