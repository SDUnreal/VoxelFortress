// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GamePlayStatics.h"
#include "GameFramework/PlayerController.h"
#include "FortressController.generated.h"

/**
 *
 */
UCLASS()
class VOXELFORTRESS_API AFortressController : public APlayerController
{
	GENERATED_BODY()

protected:


	/** Input Mapping Context to be used for player input */
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;*/

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;*/
public:
	void StartTurn();
	void FinishTurn();


	// End Actor interface
};
