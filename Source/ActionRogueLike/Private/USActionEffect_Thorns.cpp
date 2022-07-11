// Fill out your copyright notice in the Description page of Project Settings.


#include "USActionEffect_Thorns.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"


USActionEffect_Thorns::USActionEffect_Thorns()
{
	FractionMultiplier = 0.2f;
	
	Period = 0.0f;
	Duration = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChange.AddDynamic(this, &USActionEffect_Thorns::OnHealthChange);
	}
	
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChange.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChange);
	}
}

void USActionEffect_Thorns::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue,
	float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Only damage
	if (Delta < 0.0f && InstigatorActor != OwningActor)
	{
		int32 ThornDamage = FMath::RoundToInt(Delta * FractionMultiplier);
		if (ThornDamage == 0)
		{
			return;
		}

		// Flip to positive so we dont end up healing ourselves when passed into damage
		ThornDamage = FMath::Abs(ThornDamage);

		// Return Damage sender
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ThornDamage);
	}
}