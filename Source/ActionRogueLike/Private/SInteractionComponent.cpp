// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), false, TEXT("enable Debug Lines for Interact Component"));

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteraction()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	TArray<FHitResult> Hits;

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * Range);

	float Radius = 20.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation,End, ObjectQueryParams);
	
	//AActor* ActorHit = Hit.GetActor();

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, FColor::Green, false, 2.0f);
		}
		
		AActor* ActorHit = Hit.GetActor();
		if (ActorHit)
		{
			if (ActorHit->Implements<UGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
			
				IGameplayInterface::Execute_Interact(ActorHit, MyPawn);
				break;
			}
		}
	}
	
	// if we hit something (bBlockingHit ?) color is green otherwise is red
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 2.0f);	
	}
	
}

