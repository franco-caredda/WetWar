// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ServerSideRewindComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/GameStateBase.h"

#include "Interfaces/ServerSideRewindComponentInterface.h"

#include "Kismet/GameplayStatics.h"

UServerSideRewindComponent::UServerSideRewindComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UServerSideRewindComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UServerSideRewindComponent::SaveSnapshot()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
	AActor* aOwner = GetOwner();

	FSnapshot Snapshot{};
	Snapshot.Location = aOwner->GetActorLocation();
	Snapshot.Rotation = aOwner->GetActorRotation();
	Snapshot.RegistrationTime = GameState->GetServerWorldTimeSeconds();

	SnapshotHistory.Add(Snapshot);

	UE_LOG(LogActorComponent, Display, TEXT("%s Snapshot saved: %f"), *GetOwner()->GetName(),
		Snapshot.RegistrationTime);
}

void UServerSideRewindComponent::RefreshHistory()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (SnapshotHistory.IsEmpty())
	{
		return;
	}

	const FSnapshot& First = SnapshotHistory[0];
	const FSnapshot& Last = SnapshotHistory.Last();

	if (&First == &Last)
	{
		return;
	}

	float PassedTime = Last.RegistrationTime - First.RegistrationTime;
	if (PassedTime >= SnapshotLifetime)
	{
		UE_LOG(LogActorComponent, Display, TEXT("%s Snapshot is being removed; PassedTime: %f"),
			*GetOwner()->GetName(),
			PassedTime);
		
		SnapshotHistory.RemoveAt(0);
	}
}

void UServerSideRewindComponent::DrawDebugHistory()
{
	if (!bDrawDebugHistory)
	{
		return;
	}
	
	for (const auto& Snapshot : SnapshotHistory)
	{
		DrawDebugCapsule(GetWorld(), Snapshot.Location, 88.0f, 34.0f, FQuat(Snapshot.Rotation),
			FColor::Red, false, 0.0f, 0, 1.5f);
	}
}

void UServerSideRewindComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RefreshHistory();
	SaveSnapshot();
	DrawDebugHistory();
}

bool UServerSideRewindComponent::HandleDetectHit(float Time, AActor* Actor, const FVector& StartLocation,
	const FVector& WorldDirection, float TraceLength) const
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return false;
	}

	if (IServerSideRewindComponentInterface* ServerSideRewindComponentInterface =
		Cast<IServerSideRewindComponentInterface>(Actor))
	{
		UServerSideRewindComponent* ServerSideRewindComponent =
			ServerSideRewindComponentInterface->GetServerSideRewindComponent();

		// Find the closest snapshots around the given Time
		FSnapshot Older{}, Newer{};
		for (int i = 1; i < ServerSideRewindComponent->SnapshotHistory.Num(); i++)
		{
			if (ServerSideRewindComponent->SnapshotHistory[i - 1].RegistrationTime <= Time &&
				ServerSideRewindComponent->SnapshotHistory[i].RegistrationTime >= Time)
			{
				Older = ServerSideRewindComponent->SnapshotHistory[i - 1];
				Newer = ServerSideRewindComponent->SnapshotHistory[i];
				break;
			}
		}

		if (!Older.IsValid() || !Newer.IsValid())
		{
			return false;
		}

		UCapsuleComponent* CapsuleComponent =
			ServerSideRewindComponentInterface->GetCollisionCapsuleComponent();

		FVector WorldLocation = CapsuleComponent->GetComponentLocation();
		FRotator WorldRotation = CapsuleComponent->GetComponentRotation();

		// Interpolate the position between two snapshots
		float Alpha = (Time - Older.RegistrationTime) /
				  (Newer.RegistrationTime - Older.RegistrationTime);

		FVector InterpLocation = FMath::Lerp(Older.Location, Newer.Location, Alpha);
		FRotator InterpRotation = FMath::Lerp(Older.Rotation, Newer.Rotation, Alpha);

		CapsuleComponent->SetWorldLocation(InterpLocation, false,
			nullptr, ETeleportType::TeleportPhysics);
		CapsuleComponent->SetWorldRotation(InterpRotation, false,
			nullptr, ETeleportType::TeleportPhysics);
		CapsuleComponent->UpdateOverlaps();

		FHitResult Hit{};
		FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
		CollisionParams.AddIgnoredActor(GetOwner());

		// Trace against the position
		if (!GetWorld()->LineTraceSingleByChannel(
			Hit,
			StartLocation,
			StartLocation + WorldDirection * TraceLength,
			ECollisionChannel::ECC_Visibility,
			CollisionParams))
		{
			return false;
		}

		if (Hit.GetComponent() == CapsuleComponent)
		{
			if (bDrawDebugHistory)
			{
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector{5.0f, 5.0f, 5.0f},
					FColor::Green, false, 1.0f, 0, 1.0f);
			}
			
			CapsuleComponent->SetWorldLocation(WorldLocation);
			CapsuleComponent->SetWorldRotation(WorldRotation);
			return true;
		}
		
		CapsuleComponent->SetWorldLocation(WorldLocation);
		CapsuleComponent->SetWorldRotation(WorldRotation);
	}
	
	return false;
}

