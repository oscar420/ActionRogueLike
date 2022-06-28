// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SCreditCoins.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCreditCoins : public ASPowerUpBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCreditCoins();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Credit")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category="Credit")
	int32 CreditValue;

	

	
};
