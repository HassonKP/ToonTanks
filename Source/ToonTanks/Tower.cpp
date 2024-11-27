// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //Check if the tank exists

    if(InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());
    }

}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}


void ATower::BeginPlay()
{
    Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
    //Check to see if the tank is within range
    if(Tank == nullptr)
    {
        return;
    }
    
    if(InFireRange() && Tank->bAlive)
    {
        //If in range Fire
        Fire();
    }
}

bool ATower::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(),Tank->GetActorLocation());
        if(Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}