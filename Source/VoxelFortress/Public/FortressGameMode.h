// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/FortressController.h"
#include "GameFramework/GameModeBase.h"
#include "FortressGameMode.generated.h"

/*
*	이 클래스의 구조
*
*	설계 후 정리를 위해서 남겨드립니다.
*
*	이 게임 모드는 다음과 같은 룰을 가지고 있습니다.
*
*	1. Game을 시작한다
*	2. Host는 0번 째 사람의 턴을 시작한다  -> 이부분은 RPC를 통해서 선언
*	3. Client가 그 턴을 마친 경우 server에 신호를 보낸다. -> 이부분도 RPC를 통해서 선언
*	4. Host는 그 정보를 기억한 후 다음 사람에게 턴을 넘긴다.
*   5. 그 정보 변수들은 레플리케이션 되어있다.
*	6. 이 룰이 끝나면 아마 Ending을 위한 방법을 고안할듯.
*
*
*/



UCLASS(minimalapi)
class AFortressGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//init
	AFortressGameMode();

	virtual void BeginPlay() override;

	// Session에 새로운 유저가 들어오거나 나갈때 사용하는 Logic
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	//Getter Connected PlayersCount()
	UFUNCTION(BlueprintCallable)
		int32 GetConnectedPlayersCount() const;


	// turn based 로직을 짜기 위한 시스템
	UFUNCTION(BlueprintCallable)
		void TurnMaker();

	void StartPlayerTurn(APlayerController* PlayerController);
	void EndTurn();



	//사용할 변수
	UPROPERTY(BlueprintReadOnly)
		int32 ConnectedPlayersCount;
	UPROPERTY()
		int32 CurrentPlayerIndex;

	UPROPERTY(BlueprintReadWrite)
		int32 MaxGameTurn = 100;
	UPROPERTY()
		int32 CurrentGameTurn;

	UPROPERTY(BlueprintReadWrite)
		int NumPlayers;


private:
	// PlayerList
	TArray<APlayerController*> Players;

	//Timer Handle
	FTimerHandle TurnTimerHandle;
	FTimerHandle GameLogic;

};