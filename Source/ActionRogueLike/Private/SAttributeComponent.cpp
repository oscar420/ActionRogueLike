// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component"));

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxRage = 100.f;
	MaxHealth = 100.f;
	Health = MaxHealth;
	Rage = 0.f;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
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

float USAttributeComponent::GetMaxRage()
{
	return MaxRage;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	
	
	
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	OnHealthChange.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.f)
	{	
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return  ActualDelta != 0;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;
	
	Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.f)
	{
		OnRageChange.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0.f;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return  nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}