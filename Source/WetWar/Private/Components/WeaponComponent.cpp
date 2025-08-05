// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"

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

void UWeaponComponent::Reload()
{
}

void UWeaponComponent::Fire()
{
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_ListenServer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Listening...");
	}
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		PrimaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(PrimaryWeaponClass);
		
		CurrentWeapon = PrimaryWeapon;
		OnWeaponSet.Broadcast(CurrentWeapon);

		SecondaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(SecondaryWeaponClass);
		SecondaryWeapon->SetActorHiddenInGame(true);
	}
}

void UWeaponComponent::OnRep_CurrentWeapon() const
{
	OnWeaponSet.Broadcast(CurrentWeapon);
}
