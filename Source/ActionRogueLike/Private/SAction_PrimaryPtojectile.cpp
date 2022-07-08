// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_PrimaryPtojectile.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_PrimaryPtojectile::USAction_PrimaryPtojectile()
{
	HandSocketName = TEXT("Muzzle_01");
	AttackAnimDelay = 0.2f;
}

void USAction_PrimaryPtojectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
	
		UGameplayStatics::SpawnEmitterAttached(Castingeffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay,false);
	}
	
	
	
}

void USAction_PrimaryPtojectile::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		UE_LOG(LogTemp, Warning, TEXT("Intigator Action: %s"), *GetNameSafe(InstigatorCharacter));

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		
		FHitResult Hit;
		/*AActor* MyOwner = GetOwner();
			FVector ActorLocation = MyOwner->GetActorLocation();
			FRotator ActorRotation = MyOwner->GetActorRotation();*/
	
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		//FRotator CameraRotator = CamaraComp->GetComponentRotation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * Range);
		
		bool bSucces = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, QueryParams);
	
		if (bSucces)
		{
			TraceEnd = Hit.ImpactPoint;
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 32, FColor::Green, false, 2.0f);
		}
	
		FRotator CorrectRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	
		FTransform SpawnTM = FTransform(CorrectRotation, HandLocation);
		
		//spawning is always done through the world
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
		//UGameplayStatics::SpawnEmitterAttached(ShootEffect, GetMesh(), TEXT("Muzzle_01"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	}

	StopAction(InstigatorCharacter);
}
