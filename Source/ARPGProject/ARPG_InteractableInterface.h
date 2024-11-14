// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARPG_InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UARPG_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class ARPGPROJECT_API IARPG_InteractableInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* InteractingActor) = 0;
};
