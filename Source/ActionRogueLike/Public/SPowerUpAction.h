// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SPowerUpBase.h"
#include "SPowerUpAction.generated.h"

class USAction;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerUpAction : public ASPowerUpBase
{
	GENERATED_BODY()

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category="Actions")
	TSubclassOf<USAction> ActionToAdd;
	
};
