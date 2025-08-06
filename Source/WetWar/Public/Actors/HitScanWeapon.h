// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/WeaponBase.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WETWAR_API AHitScanWeapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& WorldLocation, const FVector& WorldDirection) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDistance() const { return Distance; }
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float Distance;
};
