// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_Character.h"
#include "ARPG_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API AARPG_PlayerCharacter : public AARPG_Character
{
	GENERATED_BODY()
	AARPG_PlayerCharacter();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void InteractWithObject();
};
