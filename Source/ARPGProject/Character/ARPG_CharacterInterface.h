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

	UFUNCTION(BlueprintNativeEvent)
	void SetNextCombo(const UAnimMontage* NewNextComboMontage);
	virtual void SetNextCombo_Implementation(const UAnimMontage* NewNextComboMontage) = 0;

	UFUNCTION(BlueprintNativeEvent)
	void AttackCheckBegin();
	virtual void AttackCheckBegin_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent)
	void AttackCheckEnd();
	virtual void AttackCheckEnd_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent)
	void WeaponAttach(const FName AttachSocketName);
	virtual void WeaponAttach_Implementation(const FName AttachSocketName) = 0;

	UFUNCTION(BlueprintNativeEvent)
	void WeaponEquip(const bool InEquipping);
	virtual void WeaponEquip_Implementation(const bool InEquipping) = 0;

	virtual void ParryingReaction() = 0;

	virtual void FinishAttack() = 0;

	virtual void FinishAttackDeath() = 0;

};
