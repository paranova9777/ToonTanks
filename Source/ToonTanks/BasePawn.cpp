// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurrentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turrent Mesh"));
	TurrentMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurrentMesh);
}

void ABasePawn::HandleDestruction()
{
	if (DeathParticles) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if (DeathCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShake);
	}
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	//location of the turret minus hitresult location will give vector of turret is loking at
	FVector ToTarget = LookAtTarget - TurrentMesh->GetComponentLocation(); 
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurrentMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::Fire()
{
	FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,ProjectileSpawnLocation,ProjectileSpawnRotation);
	//allows us to know what spawned the actor
	Projectile->SetOwner(this);
}