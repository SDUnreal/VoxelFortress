// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Bullet.h"
#include "Trajectory_Cylinder.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/ActorComponent.h"
#include "projectileManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VOXELFORTRESS_API UProjectileManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProjectileManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Aiming 상태를 바꿔주는 함수
	UFUNCTION(BlueprintCallable)
		bool SwitchAimingStation();


	/*
	* TODO
	* 경로를 띄워줄 particle system 탑재
	*  2023-11-30 multiplay를 위한 RPc 함수를 만들어서 spawn actor를 콜해야함
	*
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "trajectory", meta = (BlueprintType))
		FVector trajectoryStartPosition;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Fire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "trajectory", meta = (BlueprintType))
		int BulletSpeed;

	UFUNCTION(BlueprintCallable)
		void UpdateProjectilePath(const TArray<FPredictProjectilePathPointData>& PathData);

	UFUNCTION(BlueprintCallable)
		bool testFunc_ProjectilePredict();

	UFUNCTION(BlueprintCallable)
		bool TrajectoryCalculator();

	UFUNCTION(BlueprintCallable)
		bool Fire();

	UFUNCTION(BlueprintCallable)
		void BulletSpawning();

	UPROPERTY(BlueprintReadOnly)
		FString collideObjectName;

	/*
	* TODO end
	*/

private:

	bool bAiming = false;

	UObject* worldContextObject;

	UPROPERTY(Replicated)
		FPredictProjectilePathParams predictParam;

	UPROPERTY(Replicated)
		FPredictProjectilePathResult predictResult;

	UPROPERTY(Replicated)
		FVector startLocationMediator;

	UPROPERTY(Replicated)
		FVector CurLocation;

	UPROPERTY(Replicated)
		FVector finishLocationMediator;

	UPROPERTY(Replicated)
		FVector suggestedVelocity;



	TArray<ATrajectory_Cylinder*> cylinders;

	UPROPERTY(Replicated)
		bool bCanIFire = false;

};
