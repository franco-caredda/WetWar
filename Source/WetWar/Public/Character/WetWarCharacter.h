// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WetWarCharacter.generated.h"

class AWeaponBase;

class UCameraComponent;
class UWeaponComponent;

UCLASS()
class WETWAR_API AWetWarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWetWarCharacter();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponSet(AWeaponBase* Weapon);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> FirstPersonArms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	FName FirstPersonViewGripPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	FName SpectatorViewGripPoint;
};
