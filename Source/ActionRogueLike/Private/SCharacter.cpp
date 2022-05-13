// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
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

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
	FHitResult Hit;
	/*AActor* MyOwner = GetOwner();
	FVector ActorLocation = MyOwner->GetActorLocation();
	FRotator ActorRotation = MyOwner->GetActorRotation();*/
	
	FVector CameraLocation = CamaraComp->GetComponentLocation();
	FRotator CameraRotator = CamaraComp->GetComponentRotation();
	FVector End = CameraLocation + (CameraRotator.Vector() * Range);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionShape Shape;
	Shape.SetSphere(20.f);
	
	bool bSucces = GetWorld()->SweepSingleByChannel(Hit, CameraLocation, End, FQuat::Identity, ECC_GameTraceChannel1, Shape, QueryParams);

	FRotator CorrectRotation;
	if (bSucces)
	{
		CorrectRotation = (Hit.ImpactPoint - HandLocation).Rotation();	
	}

	else
	{
		CorrectRotation = (End - HandLocation).Rotation();
	}
	
	
	FTransform SpawnTM = FTransform(CorrectRotation, HandLocation);
    
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	//spawning is alwways done through the world
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteraction()
{
	InteractionComp->PrimaryInteraction();
}

void ASCharacter::UltimateAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::UltimateAttack_TimeElapsed, 0.02f);
	
}

void ASCharacter::UltimateAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
	FHitResult Hit;
	/*AActor* MyOwner = GetOwner();
	FVector ActorLocation = MyOwner->GetActorLocation();
	FRotator ActorRotation = MyOwner->GetActorRotation();*/
	
	FVector CameraLocation = CamaraComp->GetComponentLocation();
	FRotator CameraRotator = CamaraComp->GetComponentRotation();
	FVector End = CameraLocation + (CameraRotator.Vector() * Range);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionShape Shape;
	Shape.SetSphere(20.f);
	
	bool bSucces = GetWorld()->SweepSingleByChannel(Hit, CameraLocation, End, FQuat::Identity, ECC_GameTraceChannel1, Shape, QueryParams);

	FRotator CorrectRotation;
	if (bSucces)
	{
		CorrectRotation = (Hit.ImpactPoint - HandLocation).Rotation();	
	}

	else
	{
		CorrectRotation = (End - HandLocation).Rotation();
	}
	
	
	FTransform SpawnTM = FTransform(CorrectRotation, HandLocation);
    
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	//spawning is alwways done through the world
	GetWorld()->SpawnActor<AActor>(BlackHoleProjectileClass, SpawnTM, SpawnParams);
}
