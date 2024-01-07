// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Trajectory_Cylinder.h"

// Sets default values
ATrajectory_Cylinder::ATrajectory_Cylinder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;
	MeshComp->SetRelativeRotation(FRotator(90, 0, 0));
	MeshComp->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderMeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(CylinderMeshAsset.Object);
	}

	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	//DynamicMaterialInstance = UMaterialInstanceDynamic::Create(LoadObject<UMaterial>(nullptr, TEXT("/Game/Resource/RocketPath/Mat_Path.Mat_Path")), this);
	//MeshComp->SetMaterial(0, DynamicMaterialInstance);
}

// Called when the game starts or when spawned
void ATrajectory_Cylinder::BeginPlay()
{
	Super::BeginPlay();
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(LoadObject<UMaterial>(nullptr, TEXT("/Game/Resource/RocketPath/Mat_Path.Mat_Path")), this);
	MeshComp->SetMaterial(0, DynamicMaterialInstance);


}

// Called every frame
void ATrajectory_Cylinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrajectory_Cylinder::SetMaterialColor(const FLinearColor& color)
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue("BaseColor", color);
	}
}

