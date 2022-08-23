// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChange, APawn*, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

class UUserWidget;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStatedReceived;

	UPROPERTY(BlueprintAssignable)
	FOnPawnChange OnPawnChange;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	void SetupInputComponent() override;
	
	virtual void SetPawn(APawn* InPawn) override;

	void BeginPlayingState() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingstate();

	void OnRep_PlayerState() override;
};
