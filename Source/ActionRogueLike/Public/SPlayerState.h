// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditChange, APlayerState*, Playerstate, int32, NewCredit, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category="Credit")
	int32 Credits;

public:
	
	UFUNCTION(BlueprintCallable, Category="Credit")
	void AddCredit(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Credit")
	bool RemoveCredit(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category="Credit")
	FOnCreditChange OnCreditChange;
	
	
	
	
};
