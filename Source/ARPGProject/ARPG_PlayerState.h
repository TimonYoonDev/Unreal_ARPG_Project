// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPG_PlayerState.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnChangedStateValue)

UCLASS()
class ARPGPROJECT_API AARPG_PlayerState : public APlayerState
{

	GENERATED_BODY()

	AARPG_PlayerState();

	float CurrentHeath;
	float MaxHeath;

	float CurrentStamina;
	float MaxStamina;

	float BaseDamage;

public:
	void SetCurrentHeath(float NewHeath);
	void SetMaxHeath(float NewHeath);
	void SetCurrentStamina(float NewStamina);
	void SetMaxStamina(float NewStamina);
	void SetBaseDamage(float NewBaseDamage);

	float GetCurrentHeath() const;
	float GetMaxHeath() const;
	float GetCurrentStamina() const;
	float GetMaxStamina() const;
	float GetBaseDamage() const;

	FOnChangedStateValue OnChangedStateValue;
};
