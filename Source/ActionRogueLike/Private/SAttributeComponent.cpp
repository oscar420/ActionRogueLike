// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool USAttributeComponent::IsFullHealth()
{
	return Health == MaxHealth;
}

float USAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	OnHealthChange.Broadcast(nullptr, this, Health, ActualDelta);
	
	return  ActualDelta != 0.f;
}
