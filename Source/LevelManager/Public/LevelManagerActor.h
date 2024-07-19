// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManagerActor.generated.h"

DECLARE_DYNAMIC_DELEGATE(FLevelLoadedCallback);
DECLARE_DYNAMIC_DELEGATE_OneParam(FLevelChangedCallback, TSoftObjectPtr<UWorld>, CurrentDisplayLevel);

UCLASS()
class LEVELMANAGER_API ALevelManagerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelManagerActor();

	UFUNCTION(Blueprintable, BlueprintPure, meta=(WorldContext = WorldContextObject), Category = "Easy Level Manager")
	static ALevelManagerActor* GetLevelManager(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Easy Level Manager")
	void LoadAllSubLevels();

	UFUNCTION(BlueprintCallable, Category = "Easy Level Manager")
	void ChangeLevel(TSoftObjectPtr<UWorld> DisplayMainLevel);

	// LatentAction function should be UFUNCTION
	UFUNCTION()
	void OnLevelLoaded();

	void LoadLevels();
	void UpdateSubLevelVisibility(const TSoftObjectPtr<UWorld>& SubLevel, bool bShouldBeVisible) const;
	void UpdateMainLevelVisibility(TSoftObjectPtr<UWorld> MainLevel, bool bShouldBeVisible) const;
	void SetupLevelLoadedCallback(FLevelLoadedCallback Completed);
	void SetupLevelChangedCallback(FLevelChangedCallback LevelChanged);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	static ALevelManagerActor* Instance;
	FLevelLoadedCallback LevelLoadedCallback;
	FLevelChangedCallback LevelChangedCallback;

	TArray<TSoftObjectPtr<UWorld>> LevelsShouldBeLoad;
	TSoftObjectPtr<UWorld> CurrentLoadingLevel;
	bool bAllLevelLoaded;
	bool bIsLoadingLevel;

	TSoftObjectPtr<UWorld> CurrentDisplayMainLevel;
};
