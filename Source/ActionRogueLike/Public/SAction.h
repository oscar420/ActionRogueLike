// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category="Actions")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer GrantTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer BlockedTag;

	bool bIsRunning;

public:

	UPROPERTY(EditDefaultsOnly, Category="Actions")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category="Actions")
	bool CanStart(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category="Actions")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category="Actions")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Actions")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;
	
};
