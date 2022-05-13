// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComp->InitialSpeed = 8000.f;

	DelayedDetonate = 0.2f;
	TeleportDelayed = 0.2f;
}

// Called when the game starts or when spawned

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASTeleportProjectile::Explode, DelayedDetonate);
}

// Called every frame

void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTeleportProjectile::Explode_Implementation()
{
	//if we call the super the projectile will be destroyed before teleporting de character
	//Super::Explode_Implementation();

	//Clear timer if the explode was already called through another source like actorhit
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	MovementComp->StopMovementImmediately();

	EffectComp->DeactivateSystem();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_TeleportDelay;
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay, this, &ASTeleportProjectile::TeleportInstigator, TeleportDelayed);
	
}

void ASTeleportProjectile::TeleportInstigator()
{
	AActor* ActorInstigator = GetInstigator();

	if (ensure(ActorInstigator))
	{
		ActorInstigator->TeleportTo(GetActorLocation(), ActorInstigator->GetActorRotation(), false, false);
	}

	Destroy();
}

