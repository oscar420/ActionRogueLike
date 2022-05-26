// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectile::ASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = SphereComp;
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
 
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->InitialSpeed = 8000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;
}

// Called when the game starts or when spawned

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame

void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASProjectile::Explode_Implementation()
{
	//Check to make sure we aren't already being destroyed
	//adding ensure to see if we encounter this situation at all
	if (ensure(!IsPendingKill()))
	{
		ActivateEffects();
		
		Destroy();
	}
}

void ASProjectile::ActivateEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectile::OnActorHit);
}

