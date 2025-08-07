// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WetWarPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class WETWAR_API AWetWarPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWetWarPlayerController();

	UFUNCTION(BlueprintCallable)
	bool DeprojectCenterToWorld(FVector& WorldLocation, FVector& WorldDirection) const;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void OnMoveActionTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void OnLookActionTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void OnFireActionStarted(const FInputActionValue& Value);

	UFUNCTION()
	void OnFireActionCompleted(const FInputActionValue& Value);

	UFUNCTION()
	void OnInteractActionTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void OnJumpActionStarted(const FInputActionValue& Value);

	UFUNCTION()
	void OnJumpActionCompleted(const FInputActionValue& Value);
protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> JumpAction;
};
