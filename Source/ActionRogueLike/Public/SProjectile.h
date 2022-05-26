// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USoundBase;
class UCameraShakeBase;

UCLASS(Abstract) // 'Abstract' marks this class as incomplete, keeping this out of certain dropdowns windows like SpawnActor in Unreal Editor
class ACTIONROGUELIKE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	float ImpactShakeOuterRadius;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	USoundBase* ImpactSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* ImpactVFX;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	void ActivateEffects();

	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
