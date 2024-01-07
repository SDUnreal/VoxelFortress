// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/FortressController.h"
#include "EnhancedInputSubsystems.h"
#include "Character/FortressPlayerState.h"
#include "FortressGameMode.h"


void AFortressController::BeginPlay()
{

	// get the enhanced input subsystem
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	//{
	//	// add the mapping context so we get controls
	//	Subsystem->AddMappingContext(InputMappingContext, 0);

	//	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	//}
}

void AFortressController::StartTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("SD_Unreal Turn Start!!!"));
}

void AFortressController::FinishTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("SD_Unreal Turn Start!!!"));
}
