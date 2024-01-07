// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/FortressController.h"
#include "GameFramework/GameModeBase.h"
#include "FortressGameMode.generated.h"

/*
*	�� Ŭ������ ����
*
*	���� �� ������ ���ؼ� ���ܵ帳�ϴ�.
*
*	�� ���� ���� ������ ���� ���� ������ �ֽ��ϴ�.
*
*	1. Game�� �����Ѵ�
*	2. Host�� 0�� ° ����� ���� �����Ѵ�  -> �̺κ��� RPC�� ���ؼ� ����
*	3. Client�� �� ���� ��ģ ��� server�� ��ȣ�� ������. -> �̺κе� RPC�� ���ؼ� ����
*	4. Host�� �� ������ ����� �� ���� ������� ���� �ѱ��.
*   5. �� ���� �������� ���ø����̼� �Ǿ��ִ�.
*	6. �� ���� ������ �Ƹ� Ending�� ���� ����� ����ҵ�.
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

	// Session�� ���ο� ������ �����ų� ������ ����ϴ� Logic
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	//Getter Connected PlayersCount()
	UFUNCTION(BlueprintCallable)
		int32 GetConnectedPlayersCount() const;


	// turn based ������ ¥�� ���� �ý���
	UFUNCTION(BlueprintCallable)
		void TurnMaker();

	void StartPlayerTurn(APlayerController* PlayerController);
	void EndTurn();



	//����� ����
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