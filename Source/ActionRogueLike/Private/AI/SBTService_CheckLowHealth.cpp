// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (ensure(AIPawn))
		{
			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (ensure(AttributeComp))
			{
				float AIHealth = AttributeComp->GetHealth();
				UE_LOG(LogTemp, Warning, TEXT("Health: %f"), AIHealth);

				bool bIsLowHealth = AIHealth <= (AttributeComp->GetMaxHealth()/2.f);

				if (bIsLowHealth)
				{
					BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
				}
			}
		}
	}
}
