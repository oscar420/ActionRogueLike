// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAiCharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidgetLit;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASAiCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidgetLit> SpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, Category="Actions")
	USActionComponent* ActionComp;

	UPROPERTY()
	USWorldUserWidgetLit* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidgetLit> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float FlashTime;
	
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName FlashTimeParamName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category="Component")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnPawnSeen();

	void SetTargetActor(AActor* NewTarget);

	AActor* GetTargetActor() const;

	void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, class USAttributeComponent* OwningComp, float NewHealth, float Delta);
};


