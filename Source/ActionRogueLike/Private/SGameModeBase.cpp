// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include <concrt.h>
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "GameplayInterface.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
//#include "SSaveGame.h"
#include "SSaveGame.h"
#include "AI/SAiCharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;

	CreditsPerKill = 20;
	
	PlayerStateClass = ASPlayerState::StaticClass();

	DesireSpawnNumber = 10;

	MinimumDistance = 200.f;

	SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether its allowed to determined by spawn logic later in the chain...
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	SpawnPowerUps();
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAiCharacter> It(GetWorld()); It; ++It)
	{
		ASAiCharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp && AttributeComp->IsAlive()))
		{
			AttributeComp->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bot spawning disable via cvar 'SVarSpawnbots'."));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	// Returns every Actor or class of type <> in the world
	for (TActorIterator<ASAiCharacter> It(GetWorld()); It; ++It)
	{
		ASAiCharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp && AttributeComp->IsAlive()))
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.f;;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("At Maximum capacity. Ski[[ing bot spawn"));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotQueryCompleted);
	}
}

void ASGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		//track all used spawn locations
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::RespawnPlayerElpased(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);	
	}
	
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElpased", Player->GetController());

		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);	
	}

	APawn* KillerPlayer = Cast<APawn>(Killer);
	if (KillerPlayer && KillerPlayer != VictimActor)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(KillerPlayer->GetPlayerState());
		if (PS)
		{
			PS->AddCredit(CreditsPerKill);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ASGameModeBase::SpawnPowerUps()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerUpsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnPowerUpsQueryCompleted);
	}
	
}

void ASGameModeBase::OnSpawnPowerUpsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                                   EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn PowerUps Query Failed"));

		return;
	}

	if (QueryInstance)
	{
		TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

		// Get hold of the used locations.
		TArray<FVector> UsedLocations;

		int32 SpawnCounter = 0;
		while (SpawnCounter < DesireSpawnNumber && Locations.Num() > 0)
		{
			bool bIsLocationValid = true;
			int32 RandomLocationIndex = FMath::RandRange(0, (Locations.Num() - 1));

			FVector PickUpLocation = Locations[RandomLocationIndex];
			Locations.RemoveAt(RandomLocationIndex);

			for (FVector OtherLocation : UsedLocations)
			{
				float Distance = (OtherLocation - PickUpLocation).Size();
				if (Distance < MinimumDistance)
				{
					// Is not at the required distance
					bIsLocationValid = false;

					break;
				}
			}

			if (!bIsLocationValid)
			{
				//UE_LOG(LogTemp, Warning, TEXT("not a valid location"));
				continue;
			}
			
			SpawnCounter++;
			
			//UE_LOG(LogTemp, Warning, TEXT("SawnCounter: %i"), SpawnCounter);
			
			UsedLocations.Add(PickUpLocation);
			
			int32 RandomPowwerUpClassIndex = FMath::RandRange(0, PowerUpClasess.Num()-1);
			GetWorld()->SpawnActor<AActor>(PowerUpClasess[RandomPowwerUpClassIndex], PickUpLocation + FVector (0.0f,0.0f,70.f), FRotator::ZeroRotator);
		
		}
	}
}


void ASGameModeBase::WriteSaveGame()
{
	// Iterate all player states, we dont have proper ID to match yet (requires Steam or EQS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Not multiplayer jet
		}
	}
	
	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested on our gameplay Actors
		if (!Actor->Implements<UGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();
		

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTIES(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	
	}

	//CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USaveGame::StaticClass()));
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
	
}

void ASGameModeBase::LoadSaveGame()
{
	
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		// Iterate the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// Only interested in our `Gameplay` Actors
			if (!Actor->Implements<UGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);
					
					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					IGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data."));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data."));
	}
}


