// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FortressPlayerState.h"

AFortressPlayerState::AFortressPlayerState()
{

	SetReplicates(true);

}


void AFortressPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFortressPlayerState, NumUser);
	DOREPLIFETIME(AFortressPlayerState, isMyTurn);
	DOREPLIFETIME(AFortressPlayerState, TimeLimit);
	DOREPLIFETIME(AFortressPlayerState, HP);
	DOREPLIFETIME(AFortressPlayerState, Speed);
}