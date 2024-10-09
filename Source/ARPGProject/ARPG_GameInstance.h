// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableStructs.h"
#include "Engine/GameInstance.h"
#include "ARPG_GameInstance.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	UARPG_GameInstance();
private:
	TObjectPtr<UDataTable> WeaponDataTable;
	TObjectPtr<UDataTable> CombatDataTable;

public:
	bool TryGetWeaponData(const FString& RowName, FARPG_WeaponData& OutWeaponData) const;
	bool TryGetCombatData(const FString& RowName, FARPG_CombatData& OutCombatData) const;
};
