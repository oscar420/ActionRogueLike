// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPowerUpBase, public IGameplayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:
	
	void ActivatePotion();

	void PotionStatus(bool IsActive);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_SpawnDelay;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
