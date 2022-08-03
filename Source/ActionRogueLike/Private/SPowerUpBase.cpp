// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerUpBase::ASPowerUpBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	RespawnTime = 10.f;

	bIsActive = true;

	SetReplicates(true);
}

void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes
}

void ASPowerUpBase::ShowPowerUp()
{
	SetPowerUpState(true);
}

void ASPowerUpBase::HideAndCooldownPowerUp()
{
	
	SetPowerUpState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnDelay, this, &ASPowerUpBase::ShowPowerUp, RespawnTime);
}

void ASPowerUpBase::SetPowerUpState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;

	OnRep_IsActive();
}

void ASPowerUpBase::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPowerUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUpBase, bIsActive);
}