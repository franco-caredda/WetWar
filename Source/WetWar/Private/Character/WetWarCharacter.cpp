// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/WetWarCharacter.h"

#include "Actors/WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Components/WeaponComponent.h"

AWetWarCharacter::AWetWarCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->bUsePawnControlRotation = true;
	
	FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonArms");
	FirstPersonArms->AttachToComponent(CameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonArms->SetOnlyOwnerSee(true);
	FirstPersonArms->CastShadow = false;
	
	GetMesh()->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	if (WeaponComponent)
	{
		WeaponComponent->SetIsReplicated(true);
	}
}

void AWetWarCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponComponent->OnWeaponSet.AddDynamic(this, &AWetWarCharacter::OnWeaponSet);

	if (GetNetMode() == NM_ListenServer)
	{
		WeaponComponent->RequestCurrentWeapon();
	}
}

void AWetWarCharacter::OnWeaponSet(AWeaponBase* Weapon)
{
	if (IsLocallyControlled())
	{
		Weapon->AttachToComponent(FirstPersonArms,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, FirstPersonViewGripPoint);
		
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		Weapon->SetOwner(GetController());
	}

	Weapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpectatorViewGripPoint);
}
