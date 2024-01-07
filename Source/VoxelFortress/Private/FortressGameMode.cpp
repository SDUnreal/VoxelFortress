// Copyright Epic Games, Inc. All Rights Reserved.

#include "FortressGameMode.h"
#include "Character/FortressCharacter.h"
#include "Character/FortressPlayerState.h"
#include "Controller/FortressController.h"
#include "Character/testFortressCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GamePlayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"


AFortressGameMode::AFortressGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = ATestFortressCharacter::StaticClass();
	PlayerControllerClass = AFortressController::StaticClass();
	PlayerStateClass = AFortressPlayerState::StaticClass();
	GameStateClass = AGameStateBase::StaticClass();

	//DefaultPlayerStateClass
	SetReplicates(true);
	CurrentPlayerIndex = 0;
	CurrentGameTurn = 0;
}

void AFortressGameMode::BeginPlay()
{
	Super::BeginPlay();


}

void AFortressGameMode::PostLogin(APlayerController* NewPlayer)
{
	//현재 방 인원수 업데이트
	Super::PostLogin(NewPlayer);
	AFortressPlayerState* PlayerState = Cast<AFortressPlayerState>(NewPlayer->PlayerState);
	ConnectedPlayersCount++;
	PlayerState->NumUser = ConnectedPlayersCount;
	Players.Add(NewPlayer);
	//UE_LOG(LogTemp, Warning, TEXT("SDUnreal_Connected Players Count: %d"), ConnectedPlayersCount);
}

// 문제 생길 예정.
void AFortressGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ConnectedPlayersCount--;
	//TODO : Players에서 빼야하는 Player를 어떻게 구분할 것인가..?
	//UE_LOG(LogTemp, Warning, TEXT("SDUnreal_Connected Players Count: %d"), ConnectedPlayersCount);
}

int32 AFortressGameMode::GetConnectedPlayersCount() const
{
	return ConnectedPlayersCount;
}

void AFortressGameMode::TurnMaker()
{
	if (CurrentPlayerIndex == ConnectedPlayersCount)
	{
		CurrentPlayerIndex %= ConnectedPlayersCount;
		if (++CurrentGameTurn == MaxGameTurn)
		{
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("SDUnreal_Connected TurnMaker Count: %d"), CurrentGameTurn);
	StartPlayerTurn(Players[CurrentPlayerIndex]);
}

void AFortressGameMode::StartPlayerTurn(APlayerController* PlayerController)
{
	float TurnDuration = 3.0f;
	AFortressController* FPlayerController = Cast<AFortressController>(PlayerController);
	FPlayerController->StartTurn();
	AFortressPlayerState* PlayerState = Cast<AFortressPlayerState>(PlayerController->PlayerState);
	PlayerState->isMyTurn = true;

	PlayerState->TimeLimit = TurnDuration;
	UE_LOG(LogTemp, Warning, TEXT("SDUnreal_Connected StartPlayerTurn Players: %d"), CurrentPlayerIndex);
	GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &AFortressGameMode::EndTurn, TurnDuration, false);
}

void AFortressGameMode::EndTurn()
{
	AFortressController* FPlayerController = Cast<AFortressController>(Players[CurrentPlayerIndex]);
	FPlayerController->FinishTurn();
	AFortressPlayerState* PlayerState = Cast<AFortressPlayerState>(Players[CurrentPlayerIndex]->PlayerState);
	PlayerState->isMyTurn = false;
	PlayerState->TimeLimit = 0;
	UE_LOG(LogTemp, Warning, TEXT("SDUnreal_Connected StartPlayerTurn Players: %d"), CurrentPlayerIndex);
	CurrentPlayerIndex++;
	TurnMaker();
}












