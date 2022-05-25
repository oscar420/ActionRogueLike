// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"
#include "Components/SphereComponent.h"

/*
void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	IGameplayInterface::Interact_Implementation(InstigatorPawn);
}
*/

// Sets default values
ASPowerUpBase::ASPowerUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);

}

// Called when the game starts or when spawned

void ASPowerUpBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

