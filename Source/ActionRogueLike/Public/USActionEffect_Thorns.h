// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "USActionEffect_Thorns.generated.h"

class USAttributeComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	float FractionMultiplier;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta);

public:

	USActionEffect_Thorns();
	
};
