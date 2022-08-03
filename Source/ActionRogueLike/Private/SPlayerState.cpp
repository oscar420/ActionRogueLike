// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"


void ASPlayerState::AddCredit(int32 Delta)
{
	if (!ensure(Delta >= 0.f))
	{
		return;
	}

	Credits += Delta;
	
	OnCreditChange.Broadcast(this, Credits, Delta);
	
	UE_LOG(LogTemp, Warning, TEXT("Credits: %i"), Credits);
}

bool ASPlayerState::RemoveCredit(int32 Delta)
{
	if (!ensure(Delta >= 0.f))
	{
		return false;
	}
	
	if (Credits < Delta)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enogh Credits"))
		return false;
	}

	Credits -= Delta;
	
	OnCreditChange.Broadcast(this, Credits, -Delta);
	
	return  true;
}

int32 ASPlayerState::GetCredits() const
{
	return  Credits;
}

void ASPlayerState::OnRep_CreditChange(int32 OldCredit)
{
	OnCreditChange.Broadcast(this, Credits, Credits - OldCredit);

	// void ASPlayerState::MulticastCredits_Implementation(float NewCredits, float Delta)
	// {
	// 	OnCreditsChanged.Broadcast(this, NewCredits, Delta);
	// }
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}
