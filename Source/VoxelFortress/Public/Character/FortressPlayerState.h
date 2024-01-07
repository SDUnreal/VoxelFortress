// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "FortressPlayerState.generated.h"

/**
 *  게임에 필요한 수치들 저장하는 class
 */
UCLASS()
class VOXELFORTRESS_API AFortressPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AFortressPlayerState();

	UPROPERTY(Replicated, BlueprintReadWrite)
		int32 NumUser;

	UPROPERTY(Replicated, BlueprintReadOnly)
		bool isMyTurn;

	UPROPERTY(Replicated)
		int32 TimeLimit;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 HP;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 Speed;

};
