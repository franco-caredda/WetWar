// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WetWarPlayerController.h"

#include "GameFramework/Character.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Character/WetWarCharacter.h"

AWetWarPlayerController::AWetWarPlayerController()
{
}

bool AWetWarPlayerController::DeprojectCenterToWorld(FVector& WorldLocation, FVector& WorldDirection) const
{
	int ViewportWidth, ViewportHeight;
	GetViewportSize(ViewportWidth, ViewportHeight);

	return DeprojectScreenPositionToWorld(
		ViewportWidth * 0.5f,
		ViewportHeight * 0.5f,
		WorldLocation,
		WorldDirection
	);
}

void AWetWarPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (InputSubsystem)
	{
		InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AWetWarPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		&AWetWarPlayerController::OnMoveActionTriggered);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		&AWetWarPlayerController::OnLookActionTriggered);

	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this,
		&AWetWarPlayerController::OnFireActionStarted);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,
		&AWetWarPlayerController::OnFireActionCompleted);
	
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
		&AWetWarPlayerController::OnInteractActionTriggered);
}

void AWetWarPlayerController::OnMoveActionTriggered(const FInputActionValue& Value)
{
	const FVector2D Direction = Value.Get<FVector2D>();

	const FRotator Rotation = FRotator{ 0.0f, GetControlRotation().Yaw, 0.0f };
	const FRotationMatrix RotationMatrix = FRotationMatrix{Rotation};

	const FVector ForwardVector = RotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);

	if (AWetWarCharacter* aCharacter = GetPawn<AWetWarCharacter>())
	{
		aCharacter->AddMovementInput(ForwardVector, Direction.Y);
		aCharacter->AddMovementInput(RightVector, Direction.X);
	}
}

void AWetWarPlayerController::OnLookActionTriggered(const FInputActionValue& Value)
{
	const FVector2D Direction = Value.Get<FVector2D>();

	AddPitchInput(Direction.Y);
	AddYawInput(Direction.X);
}

void AWetWarPlayerController::OnFireActionStarted(const FInputActionValue& Value)
{
	if (AWetWarCharacter* aCharacter = GetPawn<AWetWarCharacter>())
	{
		aCharacter->StartFire();
	}
}

void AWetWarPlayerController::OnFireActionCompleted(const FInputActionValue& Value)
{
	if (AWetWarCharacter* aCharacter = GetPawn<AWetWarCharacter>())
	{
		aCharacter->StopFire();
	}
}

void AWetWarPlayerController::OnInteractActionTriggered(const FInputActionValue& Value)
{
	if (AWetWarCharacter* aCharacter = GetPawn<AWetWarCharacter>())
	{
		aCharacter->ServerInteract();
	}
}
