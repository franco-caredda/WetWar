// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WeaponBase.h"

#include "Net/UnrealNetwork.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>("MuzzleLocation");
	MuzzleLocation->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponBase, CurrentWaterVolume, COND_OwnerOnly);
}

void AWeaponBase::ReloadWaterVolume()
{
	CurrentWaterVolume = MaxWaterVolume;
	OnWaterVolumeChanged.Broadcast(CurrentWaterVolume, MaxWaterVolume);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentWaterVolume = MaxWaterVolume;
		OnWaterVolumeChanged.Broadcast(CurrentWaterVolume, MaxWaterVolume);
	}
}

void AWeaponBase::HandleReduceWaterVolume()
{
	CurrentWaterVolume--;
	OnWaterVolumeChanged.Broadcast(CurrentWaterVolume, MaxWaterVolume);
}

bool AWeaponBase::Trace(const FVector& StartLocation, const FVector& EndLocation, FHitResult& OutResult) const
{
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
	TraceParams.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(
		OutResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);
}

void AWeaponBase::OnRep_CurrentWaterVolume() const
{
	OnWaterVolumeChanged.Broadcast(CurrentWaterVolume, MaxWaterVolume);
}

