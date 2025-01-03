// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void ActorDied(AActor* DeadActor);

	UFUNCTION(BluePrintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

protected:
	virtual void BeginPlay() override;
	

	UFUNCTION(BlueprintCallable)
	void ExitGame();

	UFUNCTION(BlueprintCallable)
	void RestartLevel();

private:
	class ATank* Tank;
	class AToonTanksPlayerController* ToonTanksPlayerController;

	UPROPERTY(EditAnywhere)
	float StartDelay = 3.f;

	void HandleGameStart();

	int32 TargetTowers = 0;

	int32 GetTargetTowerCount();
	
};
