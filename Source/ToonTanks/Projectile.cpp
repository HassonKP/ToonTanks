// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Whatever"))
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	//Set it to root
	RootComponent = ProjectileMesh;

	TrailingParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trailing Particle"));
	TrailingParticle->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,LaunchSound,GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Callback Function
void AProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    AActor* MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();
	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		if(HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this,HitParticles,GetActorLocation(),GetActorRotation());
		}
		if(HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation());
		}
		if(HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	}
	//Dont leave any lingering projectiles
	Destroy();
}
