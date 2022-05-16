// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attributecomp"));

	AttributeComp->OnHealthChange.AddDynamic(this, &ASTargetDummy::OnHealthChange);

}

void ASTargetDummy::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	MeshComp->SetScalarParameterValueOnMaterials(TEXT("TimeToHit"), GetWorld()->GetTimeSeconds());
}
