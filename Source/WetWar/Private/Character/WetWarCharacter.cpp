// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/WetWarCharacter.h"

#include "Actors/WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Components/WeaponComponent.h"

#include "Player/WetWarPlayerController.h"

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

void AWetWarCharacter::StartFire()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	if (!TimerManager.IsTimerActive(FireTimer) || !FireTimer.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Triggering Fire");
		OnFireTick();
		TimerManager.SetTimer(FireTimer, this, &AWetWarCharacter::OnFireTick,
			WeaponComponent->GetFireRate(), true);
	}
}

void AWetWarCharacter::StopFire()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(FireTimer);
}

void AWetWarCharacter::OnFireTick()
{
	AWetWarPlayerController* aPlayerController =
		CastChecked<AWetWarPlayerController>(GetController());

	FVector WorldLocation, WorldDirection;
	aPlayerController->DeprojectCenterToWorld(WorldLocation, WorldDirection);
	
	WeaponComponent->Fire(WorldLocation, WorldDirection);
}


ECharacterMode AWetWarCharacter::GetCharacterMode() const
{
	if (IsLocallyControlled())
	{
		return ECharacterMode::FirstPerson;
	}

	return ECharacterMode::Spectator;
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

		Weapon->OnWaterVolumeChanged.AddLambda([this](int CurrentVolume, int MaxVolume)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.25f, FColor::Red,
				FString::Printf(TEXT("Current water volume: %d"), CurrentVolume));
		});
		
		return;
	}

	Weapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpectatorViewGripPoint);
}
