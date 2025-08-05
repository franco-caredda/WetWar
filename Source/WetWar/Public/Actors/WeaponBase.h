// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaterVolumeChanged, int, CurrentVolume, int, MaxVolume);

UCLASS(Abstract)
class WETWAR_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnRep_CurrentWaterVolume() const;
public:
	UPROPERTY(BlueprintAssignable)
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

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float Distance;
private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWaterVolume)
	int CurrentWaterVolume;
};
