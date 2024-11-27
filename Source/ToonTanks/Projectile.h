// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USoundBase;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//UStaticMeshComponent* ProjectileMesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "True"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "True"))
	UParticleSystemComponent* TrailingParticle;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

};
