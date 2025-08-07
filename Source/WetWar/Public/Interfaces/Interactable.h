// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WETWAR_API IInteractable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void Interact(ACharacter* Character);
	void Interact_Implementation(ACharacter* Character) { Interact_Internal(Character); }
protected:
	virtual void Interact_Internal(ACharacter* Character) PURE_VIRTUAL(IInteractable::Interact)
};
