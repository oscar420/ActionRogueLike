// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayInterface.h"
#include "SWorldUserWidgetLit.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), false, TEXT("enable Debug Lines for Interact Component"));

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Radius = 30.f;
	Range = 500.f;
	CollisionChannel = ECC_WorldDynamic;
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}

	
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	
	TArray<FHitResult> Hits;

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * Range);

	Radius = 20.0f; 
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation,End, ObjectQueryParams);
	
	//AActor* ActorHit = Hit.GetActor();

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	// Clear ref before trying to fill
	FocusedActor = nullptr;
	
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
				FocusedActor = ActorHit;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetclass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidgetLit>(GetWorld(), DefaultWidgetclass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();	
			}
			
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	// if we hit something (bBlockingHit ?) color is green otherwise is red
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 2.0f);	
	}
}

void USInteractionComponent::PrimaryInteraction()
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No Focus Actor to interact."));
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	
	IGameplayInterface::Execute_Interact(InFocus, MyPawn);
}