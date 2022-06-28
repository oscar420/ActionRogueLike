// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SAttributeComponent.h"

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
