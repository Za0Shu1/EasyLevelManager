// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentLevelSettings.h"

TArray<TSoftObjectPtr<UWorld>> UPersistentLevelSettings::GetAllSubLevels()
{
	TArray<TSoftObjectPtr<UWorld>> Result;
	for (auto MainLevel : LevelConfig)
	{
		for (auto SubLevel : MainLevel.Value.SubLevels)
		{
			Result.AddUnique(SubLevel.Level);
		}
	}
	return Result;
}
