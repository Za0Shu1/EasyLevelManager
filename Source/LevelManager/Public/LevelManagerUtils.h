// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelManagerActor.h"
#include "LevelManagerUtils.generated.h"

/**
 * 
 */
UCLASS()
class LEVELMANAGER_API ULevelManagerUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject"), Category = "Easy Level Manager")
	static void BindOnLevelLoadedCallback(UObject* WorldContextObject, FLevelLoadedCallback Completed);

	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject"), Category = "Easy Level Manager")
	static void BindOnLevelChangedCallback(UObject* WorldContextObject, FLevelChangedCallback LevelChanged);
};
