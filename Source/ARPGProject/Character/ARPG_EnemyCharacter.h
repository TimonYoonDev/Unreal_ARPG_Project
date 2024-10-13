// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_AICharacterInterface.h"
#include "ARPG_Character.h"
#include "ARPG_EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API AARPG_EnemyCharacter : public AARPG_Character, public IARPG_AICharacterInterface
{
	GENERATED_BODY()
	AARPG_EnemyCharacter();
	virtual void SetWalkSpeed_Implementation(const float InSpeed) override;
};
