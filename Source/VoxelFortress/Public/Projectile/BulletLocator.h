// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// Deprecated (실험을 위해서 한번 만들어봤는데, 그냥 궤적을 이용해서 만드는게 더 나은거 같기도?)
/**
 *  원래 하려던것
 *  탱크가 탄착군을 조절한다 -> 이러면 미리 놓을 공간을 줌인해서 거기에 놓아질 부분의 예측지점을 만들어본다.
 *
 *
 */

#include "CoreMinimal.h"
#include "Bullet.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Pawn.h"
#include "bulletLocator.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class VOXELFORTRESS_API ABulletLocator : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABulletLocator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal")
		class UDecalComponent* DecalComp;

	struct FInputActionValue;

	UPROPERTY()
		class ABullet* bullet;

	UPROPERTY(VisibleAnywhere)
		FVector Location;


};
