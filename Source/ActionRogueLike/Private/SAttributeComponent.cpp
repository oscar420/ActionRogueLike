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


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	/*if (FMath::Clamp(Health, 0.f, MaxHealth))
	{*/
		Health += Delta;
		Health = FMath::Clamp(Health, 0.f, MaxHealth);
		
		OnHealthChange.Broadcast(nullptr, this, Health, Delta);
	
		return  true;
	//}
	
	//return  true;
}
