// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWaterVolumeChanged, int, int);

/**
 * The base class for weapons that doesn't contain blueprints-exposed functions.
 * The communication between characters and weapons should be implemented through UWeaponComponent.
 */
UCLASS(Abstract)
class WETWAR_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	
	virtual void Fire(const FVector& WorldLocation, const FVector& WorldDirection) PURE_VIRTUAL(AWeaponBase::Fire);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE bool CanFire() const { return CurrentWaterVolume > 0; }
	FORCEINLINE FVector GetMuzzleLocation() const { return MuzzleLocation->GetComponentLocation(); }
	FORCEINLINE float GetFireRate() const { return FireRate; }
protected:
	virtual void BeginPlay() override;

	void HandleReduceWaterVolume();
	bool Trace(const FVector& StartLocation, const FVector& EndLocation, FHitResult& OutResult) const;
private:
	UFUNCTION()
	void OnRep_CurrentWaterVolume() const;
public:
	FOnWaterVolumeChanged OnWaterVolumeChanged;
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> MuzzleLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	int MaxWaterVolume;
private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWaterVolume)
	int CurrentWaterVolume;
};
