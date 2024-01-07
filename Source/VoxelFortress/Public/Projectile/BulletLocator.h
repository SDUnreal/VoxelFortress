// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// Deprecated (������ ���ؼ� �ѹ� �����ôµ�, �׳� ������ �̿��ؼ� ����°� �� ������ ���⵵?)
/**
 *  ���� �Ϸ�����
 *  ��ũ�� ź������ �����Ѵ� -> �̷��� �̸� ���� ������ �����ؼ� �ű⿡ ������ �κ��� ���������� ������.
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
