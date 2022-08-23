// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAiCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidgetLit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAiCharacter::ASAiCharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attributecomp"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	FlashTime = 4.f;

	TimeToHitParamName = TEXT("TimeTohit");
	FlashTimeParamName = TEXT("FlashTime");
}


void ASAiCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAiCharacter::OnPawnSeen);
	AttributeComp->OnHealthChange.AddDynamic(this, &ASAiCharacter::OnHealthChange);
}

void ASAiCharacter::OnPawnSeen(APawn* Pawn)
{
	// Ignore if Target Actor is already set
	if (!GetTargetActor())
	{
		SetTargetActor(Pawn);
		
		MulticastOnPawnSeen();
	}
	

	
}

void ASAiCharacter::MulticastOnPawnSeen_Implementation()
{
	USWorldUserWidgetLit* NewWidget = CreateWidget<USWorldUserWidgetLit>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;

		// ZOrder = 10 so the widget will spawn upfront of the healthbar (layer order)
		NewWidget->AddToViewport(10);
	}

	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}


void ASAiCharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                   float Delta)
{
	if (InstigatorActor != this)
	{
		SetTargetActor(InstigatorActor);
	}

	if (ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidgetLit>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
		
	}
	
	
	if (Delta < 0.0f)
	{

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		GetMesh()->SetScalarParameterValueOnMaterials(FlashTimeParamName, FlashTime);
		
		if (NewHealth <= 0.0f)
		{
			// stop bt
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic(TEXT("Killed"));
			}
			
			//ragdol
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			//lifespan
			SetLifeSpan(10.f);			
		}
	}
}

void ASAiCharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

AActor* ASAiCharacter::GetTargetActor() const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}

	return nullptr;
}