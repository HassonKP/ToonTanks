// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"




ATank::ATank()
{
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComp->SetupAttachment(RootComponent);
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    //Access the Bind axis function
    //Put the name of the Axis Mapping "MoveForward"
    //Second a pointer to the object we are binding the function for
    //Third the address of the function we are binding
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if(TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    //Hide the pawn, so we can still see the scene and have access to the camera but not the pawn.
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}

void ATank::BeginPlay()
{
    Super::BeginPlay();
    TankPlayerController = Cast<APlayerController>(GetController());
    //DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(0.f,0.f,200.f), 100.f, 12, FColor::Red, true, 30.f);
}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    //X = Value * DeltaTime * Speed
    DeltaLocation.X = Value * DeltaTime * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    //Yaw = Value * DeltaTime * TurnRate
    DeltaRotation.Yaw = Value * DeltaTime * TurnRate;
    AddActorLocalRotation(DeltaRotation, true);
}

