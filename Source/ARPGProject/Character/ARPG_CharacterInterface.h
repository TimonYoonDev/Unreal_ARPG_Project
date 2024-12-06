// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARPG_CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UARPG_CharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPGPROJECT_API IARPG_CharacterInterface
{
	GENERATED_BODY()

public:
	virtual void SetNextCombo(const UAnimMontage* NewNextComboMontage) = 0;

	virtual void AttackCheckBegin() = 0;

	virtual void AttackCheckEnd() = 0;

	virtual void WeaponAttach(const FName AttachSocketName) = 0;

	virtual void ParryingReaction() = 0;

	virtual void FinishAttackReaction() = 0;

	virtual void FinishAttackDeath() = 0;

	virtual void AssassinateReaction() = 0;

	virtual void SetMotionWarping(const AActor* InTarget) = 0;

	virtual void SetMotionWarping(const FVector& TargetLocation, const FRotator& TargetRotation) = 0;

};
