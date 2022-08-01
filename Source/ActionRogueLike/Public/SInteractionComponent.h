// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidgetLit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	UPROPERTY(EditDefaultsOnly, Category="UI")	
	TSubclassOf<USWorldUserWidgetLit>DefaultWidgetclass;

	UPROPERTY()
	USWorldUserWidgetLit* DefaultWidgetInstance;

	UPROPERTY()
	AActor* FocusedActor;
	
	UPROPERTY(EditDefaultsOnly, Category="Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float Range = 375.f;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float Radius;

	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteraction();

		
};
