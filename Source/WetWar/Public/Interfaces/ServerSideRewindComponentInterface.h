// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ServerSideRewindComponentInterface.generated.h"

class UServerSideRewindComponent;
class UCapsuleComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UServerSideRewindComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WETWAR_API IServerSideRewindComponentInterface
{
	GENERATED_BODY()
public:
	virtual UServerSideRewindComponent* GetServerSideRewindComponent() const = 0;
	virtual UCapsuleComponent* GetCollisionCapsuleComponent() const = 0;
};
