// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditCoins.h"
#include "SPlayerState.h"

// Sets default values
ASCreditCoins::ASCreditCoins()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditValue = 20;
}

void ASCreditCoins::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		if (ensure(PS))
		{
			PS->AddCredit(CreditValue);
			HideAndCooldownPowerUp();
		}
	}
}


