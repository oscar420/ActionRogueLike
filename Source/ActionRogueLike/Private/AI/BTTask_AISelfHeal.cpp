// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AISelfHeal.h"
#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type UBTTask_AISelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Aplaying health change"));
		AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetMaxHealth());
	}

	return EBTNodeResult::Succeeded;
}
	

