// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	bool bIsRunning;
	
};


/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	USActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category="Actions")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer GrantTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer BlockedTag;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning

	UFUNCTION()
	void OnRep_RepData();

public:

	void Initialize(USActionComponent* NewActionComp);
	
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

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	};
};
