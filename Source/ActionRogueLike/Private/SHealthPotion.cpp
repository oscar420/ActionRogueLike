// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "PhysXInterfaceWrapperCore.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values

ASHealthPotion::ASHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame

void ASHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	IGameplayInterface::Interact_Implementation(InstigatorPawn);

	if (InstigatorPawn)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->Health != AttributeComp->MaxHealth)
		{
			AttributeComp->ApplyHealthChange(50.f);
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			SetActorTickEnabled(false);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnDelay, this, &ASHealthPotion::ActivatePotion, 10.f);
		}
	}
	
	

}

void ASHealthPotion::ActivatePotion()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

