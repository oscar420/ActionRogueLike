// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CamaraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraComp"));
	CamaraComp->SetupAttachment(SpringArmComp);

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attributecomp"));

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	//AttackAnimDelay = 0.2f;
	FlashTimeParamName = TEXT("Flashtime");
	TimeToHitParamName = TEXT("TimeToHit");
	//HandSocketName = TEXT("Muzzle_01");
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChange.AddDynamic(this, &ASCharacter::OnHealthChange);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent-> BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PrimaryInteraction"), IE_Pressed, this, &ASCharacter::PrimaryInteraction);
	PlayerInputComponent->BindAction(TEXT("UltimateAttack"), IE_Pressed, this, &ASCharacter::UltimateAttack);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Pressed, this, &ASCharacter::TeleportAttack);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ASCharacter::sprintStop);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, TEXT("Sprint"));
}

void ASCharacter::sprintStop()
{
	ActionComp->StopActionByName(this, TEXT("Sprint"));
}

void ASCharacter::PrimaryAttack()
{

	ActionComp->StartActionByName(this, "PrimaryAttack");
	
	/*StartAttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);*/

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

/*void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}*/

void ASCharacter::PrimaryInteraction()
{
	InteractionComp->PrimaryInteraction();
}

void ASCharacter::UltimateAttack()
{
	ActionComp->StartActionByName(this, "BlackHole");	
}

/*void ASCharacter::UltimateAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}*/

void ASCharacter::TeleportAttack()
{
	ActionComp->StartActionByName(this, "Dash");
}

/*void ASCharacter::TeleportAttack_TimeElapsed()
{
	SpawnProjectile(TeleportProjectileClass);
}*/

// overrides teh value use in GetActorEyesViewPoint function on SInteractionComponent class and sue the camara location instead of the actoreyes viewpoint
FVector ASCharacter::GetPawnViewLocation() const
{
	return CamaraComp->GetComponentLocation();
}

/*void ASCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);
	UGameplayStatics::SpawnEmitterAttached(ShootEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}*/

/*void ASCharacter::SpawnProjectile(TSubclassOf<AActor> SpawnProjectileClass)
{
	if (ensureAlways(SpawnProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		FHitResult Hit;
		/*AActor* MyOwner = GetOwner();
		FVector ActorLocation = MyOwner->GetActorLocation();
		FRotator ActorRotation = MyOwner->GetActorRotation();#1#
	
		FVector TraceStart = CamaraComp->GetComponentLocation();
		FRotator CameraRotator = CamaraComp->GetComponentRotation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * Range);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		FCollisionShape Shape;
		Shape.SetSphere(20.f);
	
		bool bSucces = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, QueryParams);
	
		if (bSucces)
		{
			TraceEnd = Hit.ImpactPoint;
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 32, FColor::Green, false, 2.0f);
		}
	
		FRotator CorrectRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	
		FTransform SpawnTM = FTransform(CorrectRotation, HandLocation);
    
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		//spawning is always done through the world
		GetWorld()->SpawnActor<AActor>(SpawnProjectileClass, SpawnTM, SpawnParams);
		//UGameplayStatics::SpawnEmitterAttached(ShootEffect, GetMesh(), TEXT("Muzzle_01"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	}
	
}*/

void ASCharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta)
{
	if (Delta < 0.f)
	{
		USkeletalMeshComponent* MeshComp =GetMesh();
		MeshComp->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		MeshComp->SetScalarParameterValueOnMaterials(FlashTimeParamName, FlashTime);
		
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);	
	}
	
	
	if (NewValue <= 0.0f && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::HealSelf(float Amount /*100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}
