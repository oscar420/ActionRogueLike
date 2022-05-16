// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChange, AActor*, InstigatorActor, class USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute")
	float Health;
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;
	
	
	UFUNCTION(BlueprintCallable, Category="Attribute")
	bool ApplyHealthChange(float Delta);
	
};