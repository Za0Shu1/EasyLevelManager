// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManagerActor.h"

#include "EngineUtils.h"
#include "PersistentLevelSettings.h"
#include "Kismet/GameplayStatics.h"

ALevelManagerActor* ALevelManagerActor::Instance = nullptr;

// Sets default values
ALevelManagerActor::ALevelManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentLoadingLevel = nullptr;
	bAllLevelLoaded = false;
	bIsLoadingLevel = false;

	CurrentDisplayMainLevel = nullptr;
}

ALevelManagerActor* ALevelManagerActor::GetLevelManager(UObject* WorldContextObject)
{
	if (!Instance)
	{
		UWorld* World = WorldContextObject->GetWorld();

		for (TActorIterator<ALevelManagerActor> Iterator(World); Iterator; ++Iterator)
		{
			Instance = *Iterator;
			break;
		}

		if (!Instance)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Instance = World->SpawnActor<ALevelManagerActor>(StaticClass(), SpawnParameters);
		}
	}

	return Instance;
}

void ALevelManagerActor::LoadAllSubLevels()
{
	if (bIsLoadingLevel || bAllLevelLoaded) return;

	if (UPersistentLevelSettings* Settings = GetMutableDefault<UPersistentLevelSettings>())
	{
		LevelsShouldBeLoad = Settings->GetAllSubLevels();
		bIsLoadingLevel = true;
		bAllLevelLoaded = false;
		LoadLevels();
	}
}

void ALevelManagerActor::ChangeLevel(TSoftObjectPtr<UWorld> DisplayMainLevel)
{
	// already display
	if (CurrentDisplayMainLevel == DisplayMainLevel)
	{
		return;
	}

	// hide current main level firstly
	if (CurrentDisplayMainLevel)
	{
		UpdateMainLevelVisibility(CurrentDisplayMainLevel, false);
	}

	// display new main level
	UpdateMainLevelVisibility(DisplayMainLevel, true);

	CurrentDisplayMainLevel = DisplayMainLevel;
	LevelChangedCallback.ExecuteIfBound(CurrentDisplayMainLevel);
}

void ALevelManagerActor::LoadLevels()
{
	if (LevelsShouldBeLoad.IsValidIndex(0))
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("OnLevelLoaded");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = FMath::Rand();

		CurrentLoadingLevel = LevelsShouldBeLoad[0];
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, CurrentLoadingLevel, false, false, LatentInfo);
	}
	else
	{
		bIsLoadingLevel = false;
		bAllLevelLoaded = true;
		CurrentLoadingLevel = nullptr;

		if (UPersistentLevelSettings* Settings = GetMutableDefault<UPersistentLevelSettings>())
		{
			for (const auto LevelConfig : Settings->LevelConfig)
			{
				if (LevelConfig.Value.bVisibleAfterAllLevelLoad)
				{
					ChangeLevel(LevelConfig.Key);
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("All the sub-levels have been successfully loaded."));
		LevelLoadedCallback.ExecuteIfBound();
	}
}

void ALevelManagerActor::OnLevelLoaded()
{
	LevelsShouldBeLoad.RemoveAt(0);

	UE_LOG(LogTemp, Warning, TEXT("%s ==== [%s] has been loaded, there are still [%d] levels left"),
	       *FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S")),
	       *CurrentLoadingLevel.ToString(),
	       LevelsShouldBeLoad.Num());

	LoadLevels();
}

void ALevelManagerActor::UpdateSubLevelVisibility(const TSoftObjectPtr<UWorld>& SubLevel,
                                                  const bool bShouldBeVisible) const
{
	for (const auto LevelStreaming : this->GetWorld()->GetStreamingLevels())
	{
		if (LevelStreaming && LevelStreaming->GetWorldAsset()->GetPathName() == SubLevel->GetPathName())
		{
			LevelStreaming->SetShouldBeVisible(bShouldBeVisible);
			break;
		}
	}
}

void ALevelManagerActor::UpdateMainLevelVisibility(TSoftObjectPtr<UWorld> MainLevel, bool bShouldBeVisible) const
{
	if (UPersistentLevelSettings* Settings = GetMutableDefault<UPersistentLevelSettings>())
	{
		// Display new subLevels
		if (MainLevel)
		{
			if (FMainLevelConfig* MainLevelConfig = Settings->LevelConfig.Find(MainLevel))
			{
				for (const auto [Level, bVisibleByBlueprint] : MainLevelConfig->SubLevels)
				{
					UpdateSubLevelVisibility(Level, bShouldBeVisible && !bVisibleByBlueprint);
				}
				UGameplayStatics::FlushLevelStreaming(this);
			}
		}
	}
}

void ALevelManagerActor::SetupLevelLoadedCallback(FLevelLoadedCallback Completed)
{
	LevelLoadedCallback = Completed;
}

void ALevelManagerActor::SetupLevelChangedCallback(FLevelChangedCallback LevelChanged)
{
	LevelChangedCallback = LevelChanged;
}

// Called when the game starts or when spawned
void ALevelManagerActor::BeginPlay()
{
	Super::BeginPlay();

	// if place actor in level,make sure Instance Valid
	if (!Instance)
	{
		Instance = this;
	}
	else if (Instance != this)
	{
		Destroy();
	}
}

void ALevelManagerActor::BeginDestroy()
{
	Super::BeginDestroy();

	// make sure instance will not be a wild pointer
	if (Instance == this)
	{
		Instance = nullptr;
	}
}

// Called every frame
void ALevelManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
