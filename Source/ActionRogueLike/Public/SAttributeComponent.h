// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChange, AActor*, InstigatorActor, class USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChange, AActor*, InstigatorActor, class USAttributeComponent*, OwningComp, float, NewRage, float, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributechange, AActor*, InstigatorActor, class USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	// Sets default values for this component's properties
	USAttributeComponent();

	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attribute")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute")
	float MaxRage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute")
	float MaxHealth;

	

	
public:

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributechange OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnAttributechange OnRageChange;
	
	UFUNCTION(BlueprintCallable, Category="Attribute")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attribute")
	bool ApplyRage(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attribute")
	bool IsFullHealth();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	float GetMaxRage();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category="Attribute")
	float GetRage() const;
};


