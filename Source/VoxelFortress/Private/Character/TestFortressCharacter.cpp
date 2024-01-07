// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestFortressCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "Net/UnrealNetwork.h"
#include "Projectile/projectileManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

// Sets default values
ATestFortressCharacter::ATestFortressCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
	RootComponent = SphereComp;
	SphereComp->InitSphereRadius(5.0f);

	GetMovementComponent()->UpdatedComponent = RootComponent;

	ProjectileManager = CreateDefaultSubobject<UProjectileManager>(TEXT("ProjectileManager"));
	if (ProjectileManager)
	{
		ProjectileManager->SetIsReplicated(true);
	}

	/*MoveComp = CreateDefaultSubobject<UCharacterMovementComponent>("MoveComp");
	MoveComp->UpdatedComponent = RootComponent;
	if (MoveComp)
	{
		MoveComp->SetIsReplicated(true);
	}*/

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(SphereComp);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;


	static ConstructorHelpers::FObjectFinder<UInputMappingContext>PLAYCONTEXT(TEXT("/Game/EnhancedInputSystem/FortressInputContext.FortressInputContext"));
	if (PLAYCONTEXT.Succeeded())
		DefaultContext = PLAYCONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>MoveAction(TEXT("/Game/EnhancedInputSystem/IA_Move2.IA_Move2"));
	if (MoveAction.Succeeded())
		Move = MoveAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>TurnAction(TEXT("/Game/EnhancedInputSystem/IA_Turn2.IA_Turn2"));
	if (TurnAction.Succeeded())
		Turn = TurnAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>FireAction(TEXT("/Game/EnhancedInputSystem/IA_Fire.IA_Fire"));
	if (FireAction.Succeeded())
		Fire = FireAction.Object;
}

// Called when the game starts or when spawned
void ATestFortressCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(DefaultContext, 0);
		}
	}
}

// Called every frame
void ATestFortressCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestFortressCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &ATestFortressCharacter::MoveAction);
		EnhancedInputComponent->BindAction(Turn, ETriggerEvent::Triggered, this, &ATestFortressCharacter::TurnAction);
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Triggered, this, &ATestFortressCharacter::PredictBullet);
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Completed, this, &ATestFortressCharacter::FireAction);
	}
}

void ATestFortressCharacter::MoveAction(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector NewLocation = GetActorLocation() + GetActorRightVector() * MovementVector.X + GetActorForwardVector() * MovementVector.Y;
	//SetActorLocation(NewLocation);


	if (Controller != nullptr)
	{
		// add movement 

		AddMovementInput(GetActorForwardVector() * MovementVector.Y);
		AddMovementInput(GetActorRightVector() * MovementVector.X);
		UE_LOG(LogTemp, Warning, TEXT("MoveAction - X: %f, Y: %f, "), NewLocation.X, NewLocation.Y);
	}

}

void ATestFortressCharacter::TurnAction(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	/*FRotator NewRotator = GetActorRotation() + FRotator(0, LookAxisVector.X * 5.0f,0  );

	SetActorRotation(NewRotator);*/


	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Helloe"))
	}
}

void ATestFortressCharacter::PredictBullet(const FInputActionValue& Value)
{
	bool ReadyToPredict = Value.Get<bool>();
	if (ReadyToPredict)
	{
		ProjectileManager->TrajectoryCalculator();
	}
}

void ATestFortressCharacter::FireAction(const FInputActionValue& Value)
{
	bool ReadyToFire = Value.Get<bool>();
	if (ReadyToFire != true)
	{
		ProjectileManager->Fire();
	}
}

void ATestFortressCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

