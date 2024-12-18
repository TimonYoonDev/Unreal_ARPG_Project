// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"

enum EAttackType
{
	None = 0,
	LightAttack,
	HeavyAttack,
};

enum class EDamageType
{
	None = 0,
	Hit,
	KnockBack,
	KnockDown,
};

struct FARPG_AttackData
{
	EAttackType AttackType;
	float Damage;
};

struct FARPG_DamageEvent : public FDamageEvent
{
	EDamageType DamageType;
	float Damage;
	FHitResult HitInfo;

	static constexpr int32 ClassID = 3;

	virtual int32 GetTypeID() const override { return FARPG_DamageEvent::ClassID; };
	virtual bool IsOfType(int32 InID) const override { return (FARPG_DamageEvent::ClassID == InID) || FDamageEvent::IsOfType(InID); };
};