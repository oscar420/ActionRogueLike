// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include <concrt.h>
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAiCharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;

	CreditsPerKill = 20;
	
	PlayerStateClass = ASPlayerState::StaticClass();

	DesireSpawnNumber = 10;

	MinimumDistance = 200.f;
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
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disable via cvar 'SVarSpawnbots'."));
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
				UE_LOG(LogTemp, Warning, TEXT("not a valid location"));
				continue;
			}
			
			SpawnCounter++;
			
			UE_LOG(LogTemp, Warning, TEXT("SawnCounter: %i"), SpawnCounter);
			
			UsedLocations.Add(PickUpLocation);
			
			int32 RandomPowwerUpClassIndex = FMath::RandRange(0, PowerUpClasess.Num()-1);
			GetWorld()->SpawnActor<AActor>(PowerUpClasess[RandomPowwerUpClassIndex], PickUpLocation, FRotator::ZeroRotator);
		
		}
	}
}