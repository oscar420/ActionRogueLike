// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditChange, APlayerState*, Playerstate, int32, NewCredit, int32, Delta);

class USSaveGame;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(ReplicatedUsing="OnRep_CreditChange", EditDefaultsOnly, Category="Credit")
	int32 Credits;

	UFUNCTION()
	void OnRep_CreditChange(int32 OldCredit);

	// Downside of using multicast here is that we send over more data over the net, since it's an RPC with two parameters. OnRep_ is "free" since Credits is already getting replicated anyway.
	//UFUNCTION(NetMulticast, Unreliable)
	//void MulticastCredits(float NewCredits, float Delta);

public:

	UFUNCTION(BlueprintCallable, Category="Credit")
	int32 GetCredits() const;
	
	UFUNCTION(BlueprintCallable, Category="Credit")
	void AddCredit(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Credit")
	bool RemoveCredit(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCreditChange OnCreditChange;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);
	
	
	
	
};
