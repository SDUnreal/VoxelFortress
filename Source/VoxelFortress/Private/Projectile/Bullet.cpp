// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Bullet.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Resource/Rocket/SM_Missle.SM_Missle'"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	// Set the properties of the Sphere Component if needed
	MeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	MeshComp->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	bReplicates = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->UpdatedComponent = RootComponent;
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

