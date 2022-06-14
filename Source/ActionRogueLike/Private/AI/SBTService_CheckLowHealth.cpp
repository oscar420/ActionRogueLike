// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckLowHealth::USBTService_CheckLowHealth()
{
	LowHealthFraction = 0.3f;
}

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
				bool bIsLowHealth = (AttributeComp->GetHealth()/AttributeComp->GetMaxHealth()) < LowHealthFraction;

				BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
			}
		}
	}
}
