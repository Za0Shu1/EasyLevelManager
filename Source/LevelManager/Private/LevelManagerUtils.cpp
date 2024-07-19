// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManagerUtils.h"


void ULevelManagerUtils::BindOnLevelLoadedCallback(UObject* WorldContextObject, FLevelLoadedCallback Completed)
{
	if (ALevelManagerActor* LevelManagerActor = ALevelManagerActor::GetLevelManager(WorldContextObject))
	{
		LevelManagerActor->SetupLevelLoadedCallback(Completed);
	}
}

void ULevelManagerUtils::BindOnLevelChangedCallback(UObject* WorldContextObject, FLevelChangedCallback LevelChanged)
{
	if (ALevelManagerActor* LevelManagerActor = ALevelManagerActor::GetLevelManager(WorldContextObject))
	{
		LevelManagerActor->SetupLevelChangedCallback(LevelChanged);
	}
}
