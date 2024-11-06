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
	TMap<FString, UDataTable* > DataTable;
	TObjectPtr<UDataTable> WeaponDataTable;
	TObjectPtr<UDataTable> CombatDataTable;
	TObjectPtr<UDataTable> CharacterDataTable;
	TObjectPtr<UDataTable> MontageDataTable;

public:
	bool TryGetWeaponData(const FString& RowName, FARPG_WeaponData& OutWeaponData) const;
	bool TryGetCombatData(const FString& RowName, FARPG_CombatData& OutCombatData) const;
	bool TryGetCharacterData(const FString& RowName, FARPG_CharacterData& OutData) const;
	bool TryGetMontageData(const FString& RowName, FARPG_MontageData& OutData) const;
};
