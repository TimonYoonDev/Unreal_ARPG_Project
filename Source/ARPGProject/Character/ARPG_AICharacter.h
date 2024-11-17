// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_AICharacterInterface.h"
#include "ARPG_Character.h"
#include "ARPG_AICharacter.generated.h"
 
/**
 * 
 */
UCLASS()
class ARPGPROJECT_API AARPG_AICharacter : public AARPG_Character, public IARPG_AICharacterInterface
{
	GENERATED_BODY()

	AARPG_AICharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void AttackCheckBegin_Implementation() override;
	virtual void AttackCheckEnd_Implementation() override;

	bool bIsAttacking;
	TSet<FString> TargetSet;

	void AttackTrace();

	virtual void SetWalkSpeed(const float InSpeed) override;
	virtual void SetDefence(const bool bIsDefence) override;
};
