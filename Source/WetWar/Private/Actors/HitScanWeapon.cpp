// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HitScanWeapon.h"

#include "Engine/DamageEvents.h"

#include "GameFramework/Character.h"


void AHitScanWeapon::Fire(const FVector& WorldLocation, const FVector& WorldDirection)
{
	if (!CanFire())
	{
		return;
	}
	
	FHitResult ForwardTrace{};

	if (!Trace(WorldLocation,
		WorldLocation + WorldDirection * Distance, ForwardTrace))
	{
		return;
	}

	FHitResult HitResult{};
	const FVector StartLocation = GetMuzzleLocation();
	const FVector EndLocation   = ForwardTrace.Location + WorldDirection * 0.001;

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue,
		false, 0.25f, 0, 1.5f);

	if (GetLocalRole() == ROLE_Authority)
	{
		HandleReduceWaterVolume();
	}
	
	if (Trace(StartLocation, EndLocation, HitResult))
	{
		if (AActor* Actor = HitResult.GetActor())
		{
			// TODO: Add hit reaction
			if (GetLocalRole() == ROLE_Authority)
			{
				Actor->TakeDamage(Damage, FPointDamageEvent{},
				Cast<ACharacter>(GetOwner())->GetController(), this);
			}
		}
	}
}
