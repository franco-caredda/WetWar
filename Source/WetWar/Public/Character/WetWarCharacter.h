// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ServerSideRewindComponentInterface.h"
#include "WetWarCharacter.generated.h"

class UWetnessComponent;
class IInteractable;
class AWeaponBase;

class UCameraComponent;
class UWeaponComponent;
class USphereComponent;
class UServerSideRewindComponent;

UENUM(BlueprintType)
enum class ECharacterMode : uint8
{
	FirstPerson = 0,
	Spectator   = 1,

	Max
};

UCLASS()
class WETWAR_API AWetWarCharacter : public ACharacter, public IServerSideRewindComponentInterface
{
	GENERATED_BODY()

public:
	AWetWarCharacter();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	virtual UServerSideRewindComponent* GetServerSideRewindComponent() const override;
	virtual UCapsuleComponent* GetCollisionCapsuleComponent() const override;
	
	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(Server, Reliable)
	void ServerInteract();
	
	UFUNCTION(BlueprintPure)
	ECharacterMode GetCharacterMode() const;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponSet(AWeaponBase* Weapon);

	UFUNCTION()
	void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnWetnessLevelChanged(float WetnessLevel, float MaxWetnessLevel);
	
	UFUNCTION()
	void OnFireTick();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> FirstPersonArms;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWetnessComponent> WetnessComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UServerSideRewindComponent> ServerSideRewindComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	FName FirstPersonViewGripPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	FName SpectatorViewGripPoint;
private:
	FTimerHandle FireTimer;

	UPROPERTY()
	TObjectPtr<AActor> InteractActor;
};
