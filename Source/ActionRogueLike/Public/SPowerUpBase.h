 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerUpBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUpBase : public AActor, public IGameplayInterface
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	ASPowerUpBase();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_IsActive", BlueprintReadOnly)
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

	UPROPERTY(EditAnywhere, Category="PowerUp")
	float RespawnTime;

	FTimerHandle TimerHandle_SpawnDelay;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp; 

	UFUNCTION()
	void ShowPowerUp();

	void HideAndCooldownPowerUp();

	void SetPowerUpState(bool bNewIsActive);

public:
	
	void Interact_Implementation(APawn* InstigatorPawn) override;

};
