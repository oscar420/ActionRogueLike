// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpAction.h"
#include "SAction.h"
#include "SActionComponent.h"


void ASPowerUpAction::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (!ensure(InstigatorPawn && ActionToAdd))
	{
		return;
	}

	USActionComponent* ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComp && ActionComp->GetAction(ActionToAdd))
	{
		// Action already added
		return;
	}

	ActionComp->AddAction(InstigatorPawn, ActionToAdd);
	
}
