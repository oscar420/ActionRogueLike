// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_PrimaryPtojectile.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_PrimaryPtojectile : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackAnimDelay;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere)
	float Range = 10000000.f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* ShootEffect;

	// Particle system played during attack animation
	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* Castingeffect;
	
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:
	
	USAction_PrimaryPtojectile();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
