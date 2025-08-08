// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ServerSideRewindComponent.generated.h"

USTRUCT()
struct FSnapshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float RegistrationTime = 0;
	
	UPROPERTY(EditAnywhere)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FRotator Rotation = FRotator::ZeroRotator;

	FORCEINLINE bool IsValid() const
	{
		return !FMath::IsNearlyZero(RegistrationTime);
	}
};

/**
 * UServerSideRewindComponent handles the lag compensation by registering the snapshots of the owner's hitboxes.
 * This component can be interfaced only through the C++ side.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WETWAR_API UServerSideRewindComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UServerSideRewindComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Checks if the line trace hit the Actor. Should be called on the server.
	 * @param Time when the trace occured
	 * @param Actor Actor traced against. NOTE: should implement IServerSideComponentInterface
	 * @param StartLocation Start location of the trace
	 * @param EndLocation End location of the trace
	 * @return Whether the actor was hit or not
	 */
	bool HandleDetectHit(float Time, AActor* Actor, const FVector& StartLocation, const FVector& WorldDirection, float TraceLength) const;
protected:
	virtual void BeginPlay() override;

	void SaveSnapshot();
	void RefreshHistory();
	void DrawDebugHistory();
protected:
	UPROPERTY(EditDefaultsOnly, Category="Shapshot Configuration")
	float SnapshotLifetime = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category="Shapshot Configuration")
	float SnapshotTimeTolerance = 0.1;
	
	UPROPERTY(EditDefaultsOnly, Category="Shapshot Configuration")
	bool bDrawDebugHistory = false;
private:
	// TDoubleLinkedList<FSnapshot> SnapshotHistory;
	TArray<FSnapshot> SnapshotHistory;
};
