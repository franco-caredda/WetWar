// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"

#include "Actors/HitScanWeapon.h"
#include "Actors/WeaponBase.h"
#include "Net/UnrealNetwork.h"

UWeaponComponent::UWeaponComponent()
{
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, CurrentWeapon);
}

void UWeaponComponent::RequestCurrentWeapon()
{
	OnWeaponSet.Broadcast(CurrentWeapon);
}

void UWeaponComponent::ServerSwapWeapons_Implementation()
{
	CurrentWeapon->SetActorHiddenInGame(true);
	
	if (CurrentWeapon == SecondaryWeapon)
	{
		CurrentWeapon = PrimaryWeapon;
	}
	else
	{
		CurrentWeapon = SecondaryWeapon;
	}

	CurrentWeapon->SetActorHiddenInGame(false);
	OnWeaponSet.Broadcast(CurrentWeapon);
}

void UWeaponComponent::ServerReload_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ReloadWaterVolume();
	}
}

void UWeaponComponent::Fire(const FVector& WorldLocation, const FVector& WorldDirection)
{
	if (Cast<AHitScanWeapon>(CurrentWeapon) != nullptr &&
		GetOwnerRole() != ROLE_Authority)
	{
		PerformFire(WorldLocation, WorldDirection);
	}

	ServerPerformFire(WorldLocation, WorldDirection);
}

float UWeaponComponent::GetFireRate() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->GetFireRate();
	}

	return 0.0f;
}

void UWeaponComponent::PerformFire(const FVector& WorldLocation, const FVector& WorldDirection) const
{
	CurrentWeapon->Fire(WorldLocation, WorldDirection);
}

void UWeaponComponent::ServerPerformFire_Implementation(const FVector& WorldLocation, const FVector& WorldDirection)
{
	PerformFire(WorldLocation, WorldDirection);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		PrimaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(PrimaryWeaponClass);
		
		CurrentWeapon = PrimaryWeapon;
		OnWeaponSet.Broadcast(CurrentWeapon);

		SecondaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(SecondaryWeaponClass);
		SecondaryWeapon->SetActorHiddenInGame(true);

		PrimaryWeapon->SetOwner(GetOwner());
		SecondaryWeapon->SetOwner(GetOwner());
	}
}

void UWeaponComponent::OnRep_CurrentWeapon() const
{
	OnWeaponSet.Broadcast(CurrentWeapon);
}
