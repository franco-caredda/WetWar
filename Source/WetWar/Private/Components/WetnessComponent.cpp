// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WetnessComponent.h"

#include "Net/UnrealNetwork.h"

UWetnessComponent::UWetnessComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UWetnessComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWetnessComponent, WetnessLevel);
}

void UWetnessComponent::TakeDamage(float Damage)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		WetnessLevel = FMath::Clamp(WetnessLevel + Damage, 0.0f, MaxWetnessLevel);
		OnWetnessLevelChanged.Broadcast(WetnessLevel, MaxWetnessLevel);
	}
}

void UWetnessComponent::OnRep_WetnessLevel() const
{
	OnWetnessLevelChanged.Broadcast(WetnessLevel, MaxWetnessLevel);
}

void UWetnessComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		WetnessLevel = 0.0f;
		OnWetnessLevelChanged.Broadcast(WetnessLevel, MaxWetnessLevel);
	}
}

