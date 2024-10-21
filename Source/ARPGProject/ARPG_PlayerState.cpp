// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PlayerState.h"

#include "Kismet/KismetSystemLibrary.h"

AARPG_PlayerState::AARPG_PlayerState()
{
	CurrentHeath = 100;
	MaxHeath = 100;
	CurrentStamina = 100;
	MaxStamina = 100;
	BaseDamage = 30;
}

void AARPG_PlayerState::SetCurrentHeath(const float NewHeath)
{
	CurrentHeath = NewHeath;
	OnChangedStateValue.Broadcast();
}

void AARPG_PlayerState::SetMaxHeath(const float NewHeath)
{
	MaxHeath = NewHeath;
	OnChangedStateValue.Broadcast();
}

void AARPG_PlayerState::SetCurrentStamina(const float NewStamina)
{
	CurrentStamina = NewStamina;
	OnChangedStateValue.Broadcast();
}

void AARPG_PlayerState::SetMaxStamina(const float NewStamina)
{
	MaxStamina = NewStamina;
	OnChangedStateValue.Broadcast();
}

void AARPG_PlayerState::SetBaseDamage(const float NewBaseDamage)
{
	BaseDamage = NewBaseDamage;
	OnChangedStateValue.Broadcast();
}

float AARPG_PlayerState::GetCurrentHeath() const
{
	return CurrentHeath;
}

float AARPG_PlayerState::GetMaxHeath() const
{
	return MaxHeath;
}

float AARPG_PlayerState::GetCurrentStamina() const
{
	return CurrentStamina;
}

float AARPG_PlayerState::GetMaxStamina() const
{
	return MaxStamina;
}

float AARPG_PlayerState::GetBaseDamage() const
{
	return BaseDamage;
}
