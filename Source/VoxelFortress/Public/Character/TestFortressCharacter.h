// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Projectile/ProjectileManager.h"
#include "GameFramework/Character.h"
#include "TestFortressCharacter.generated.h"

UCLASS()
class VOXELFORTRESS_API ATestFortressCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestFortressCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = mesh)
		class UCapsuleComponent* CapsuleComp;

	// InputAction
	void MoveAction(const FInputActionValue& Value);

	void TurnAction(const FInputActionValue& Value);

	void PredictBullet(const FInputActionValue& Value);

	void FireAction(const FInputActionValue& Value);




	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UProjectileManager* ProjectileManager;

private:
	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = input)
		class UInputAction* Move;

	UPROPERTY(VisibleAnywhere, Category = input)
		class UInputAction* Turn;

	UPROPERTY(VisibleAnywhere, Category = input)
		class UInputAction* Fire;

	UPROPERTY(VisibleAnywhere, Category = mesh)
		class USphereComponent* SphereComp;
};
