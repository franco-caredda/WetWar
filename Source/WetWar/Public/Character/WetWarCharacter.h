// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WetWarCharacter.generated.h"

class AWeaponBase;

class UCameraComponent;
class UWeaponComponent;

UENUM(BlueprintType)
enum class ECharacterMode : uint8
{
	FirstPerson = 0,
	Spectator   = 1,

	Max
};

UCLASS()
class WETWAR_API AWetWarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWetWarCharacter();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();
	
	UFUNCTION(BlueprintPure)
	ECharacterMode GetCharacterMode() const;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponSet(AWeaponBase* Weapon);

	UFUNCTION()
	void OnFireTick();
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
private:
	FTimerHandle FireTimer;
};
