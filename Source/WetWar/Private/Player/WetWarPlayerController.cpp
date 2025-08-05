// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WetWarPlayerController.h"

#include "GameFramework/Character.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

AWetWarPlayerController::AWetWarPlayerController()
{
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

	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this,
		&AWetWarPlayerController::OnFireActionStarted);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,
		&AWetWarPlayerController::OnFireActionCompleted);
	
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
		&AWetWarPlayerController::OnInteractActionCompleted);
}

void AWetWarPlayerController::OnMoveActionTriggered(const FInputActionValue& Value)
{
	const FVector2D Direction = Value.Get<FVector2D>();

	const FRotator Rotation = FRotator{ 0.0f, GetControlRotation().Yaw, 0.0f };
	const FRotationMatrix RotationMatrix = FRotationMatrix{Rotation};

	const FVector ForwardVector = RotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);

	if (ACharacter* aCharacter = GetPawn<ACharacter>())
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
}

void AWetWarPlayerController::OnFireActionCompleted(const FInputActionValue& Value)
{
}

void AWetWarPlayerController::OnInteractActionCompleted(const FInputActionValue& Value)
{
}
