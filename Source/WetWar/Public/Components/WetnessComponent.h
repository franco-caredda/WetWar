// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WetnessComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWetnessLevelChanged, float, WetnessLevel, float, MaxWetnessLevel);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WETWAR_API UWetnessComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UWetnessComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage);
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetWetnessLevel() const { return WetnessLevel; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxWetnessLevel() const { return MaxWetnessLevel; }
protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnRep_WetnessLevel() const;
public:
	UPROPERTY(BlueprintAssignable)
	FOnWetnessLevelChanged OnWetnessLevelChanged;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float MaxWetnessLevel;
private:
	UPROPERTY(ReplicatedUsing = OnRep_WetnessLevel)
	float WetnessLevel;
};
