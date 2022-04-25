// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "PhysXInterfaceWrapperCore.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	ForceComp->SetupAttachment(Mesh);
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->Radius = 750.0f;

	// Leaving this on applies small constant force via component 'tick' (Optional)
	ForceComp->SetAutoActivate(false);

	// Optional, Ignore 'Mass' of other objects (if false, the impulse strength will be mush higher to push most objects depending on the Mass)
	ForceComp->bImpulseVelChange = true;

}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Mesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame

void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
}


