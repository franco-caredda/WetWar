// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSet, AWeaponBase*, Weapon);

/**
 *	UWeaponComponent servers as a communication point between character and weapon.
 *	It manages the current weapon, server-side shooting and client-side prediction.
 */
UCLASS()
class WETWAR_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWeaponComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void RequestCurrentWeapon();
	
	UFUNCTION(Server, Reliable)
	void ServerSwapWeapons();

	UFUNCTION(BlueprintCallable)
	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void Fire(const FVector& WorldLocation, const FVector& WorldDirection);

	UFUNCTION(BlueprintPure)
	float GetFireRate() const;
protected:
	virtual void BeginPlay() override;

	void PerformFire(const FVector& WorldLocation, const FVector& WorldDirection) const;

	UFUNCTION(Server, Reliable)
	void ServerPerformFire(const FVector& WorldLocation, const FVector& WorldDirection);
private:
	UFUNCTION()
	void OnRep_CurrentWeapon() const;
public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponSet OnWeaponSet;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Default Weapons")
	TSubclassOf<AWeaponBase> PrimaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Default Weapons")
	TSubclassOf<AWeaponBase> SecondaryWeaponClass;
private:
	UPROPERTY()
	TObjectPtr<AWeaponBase> PrimaryWeapon;

	UPROPERTY()
	TObjectPtr<AWeaponBase> SecondaryWeapon;
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon)
	TObjectPtr<AWeaponBase> CurrentWeapon;
};

