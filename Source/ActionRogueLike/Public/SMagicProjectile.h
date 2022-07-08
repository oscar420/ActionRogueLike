// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectile.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundBase;
class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TSubclassOf<USActionEffect> ActionEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	UAudioComponent* FlightLoopAudio;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount = 20.f;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};
