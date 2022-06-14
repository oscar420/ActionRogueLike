// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAiCharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASAiCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category="Component")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	void SetTargetActor(AActor* NewTarget);

	void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, class USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
