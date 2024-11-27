// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "TimerManager.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        --TargetTowers;
        if(TargetTowers == 0)
        {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));

    StartGame();

    if(ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);
    
        FTimerHandle PlayerEnableTimerHandle;
        //Set a callback passing parameters
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState,true);
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,PlayerEnableTimerDelegate,StartDelay,false);
    }

}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this,ATower::StaticClass(),Towers);
    return Towers.Num();
}

void AToonTanksGameMode::RestartLevel()
{
    UWorld* World = GetWorld();
    if (World)
    {
        // Get the current level name
        FString CurrentLevelName = World->GetMapName();

        // Remove the _C suffix if present (Blueprint-generated names)
        if (CurrentLevelName.EndsWith("_C"))
        {
            CurrentLevelName = CurrentLevelName.LeftChop(2);
        }

        // Restart the current level
        UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get UWorld in RestartLevel!"));
    }
}

void AToonTanksGameMode::ExitGame()
{
    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
        {
            // Quit the game
            UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("PlayerController is null in ExitGame!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get UWorld in ExitGame!"));
    }
}
