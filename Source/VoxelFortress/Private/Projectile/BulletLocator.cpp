// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/BulletLocator.h"

// Sets default values
ABulletLocator::ABulletLocator()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->TargetArmLength = 500.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeRotation(FRotator(0.0f, 45.0f, 0.0f));
	SpringArmComp->SetupAttachment(MeshComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);


}

// Called when the game starts or when spawned
void ABulletLocator::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ABulletLocator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABulletLocator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);



}

