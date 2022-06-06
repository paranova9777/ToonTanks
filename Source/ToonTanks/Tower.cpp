// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckTankInRange())
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

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	//sets timer on function when begin play is started 
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}
	if (CheckTankInRange() && Tank->bAlive)
	{
		Fire();
	}
}

bool ATower::CheckTankInRange()
{
	if (Tank) {
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}