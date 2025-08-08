// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HitScanWeapon.h"

#include "Components/ServerSideRewindComponent.h"

#include "Engine/DamageEvents.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"

#include "Interfaces/ServerSideRewindComponentInterface.h"
#include "Kismet/GameplayStatics.h"


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

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
	if (Trace(StartLocation, EndLocation, HitResult))
	{
		if (AActor* Actor = HitResult.GetActor())
		{
			// TODO: Add hit reaction
			ServerFire(GameState->GetServerWorldTimeSeconds(), Actor, WorldLocation, WorldDirection, GetDistance());
		}
	}
}

void AHitScanWeapon::ServerFire_Implementation(float Time, AActor* Actor, const FVector& StartLocation,
	const FVector& WorldDirection, float TraceLength)
{
	if (IServerSideRewindComponentInterface* ServerSideRewind =
					Cast<IServerSideRewindComponentInterface>(GetOwner()))
	{
		UServerSideRewindComponent* RewindComponent =
			ServerSideRewind->GetServerSideRewindComponent();

		if (RewindComponent->HandleDetectHit(Time,
			Actor, StartLocation, WorldDirection, TraceLength))
		{
			Actor->TakeDamage(Damage, FPointDamageEvent{},
		Cast<ACharacter>(GetOwner())->GetController(), this);
		}
	}
}

