// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_GameInstance.h"

UARPG_GameInstance::UARPG_GameInstance()
{
	if(const ConstructorHelpers::FObjectFinder<UDataTable> DT_WeaponData(TEXT("/Script/Engine.DataTable'/Game/ARPG/GameData/DT_WeaponData.DT_WeaponData'")); DT_WeaponData.Succeeded())
	{
		WeaponDataTable = DT_WeaponData.Object;
	}

	if(const ConstructorHelpers::FObjectFinder<UDataTable> DT_CombatData(TEXT("/Script/Engine.DataTable'/Game/ARPG/GameData/DT_CombatData.DT_CombatData'")); DT_CombatData.Succeeded())
	{
        CombatDataTable = DT_CombatData.Object;
	}

	if(const ConstructorHelpers::FObjectFinder<UDataTable> DT_CharacterData(TEXT("/Script/Engine.DataTable'/Game/ARPG/GameData/DT_CharacterData.DT_CharacterData'")); DT_CharacterData.Succeeded())
	{
        CharacterDataTable = DT_CharacterData.Object;
	}
}

bool UARPG_GameInstance::TryGetWeaponData(const FString& RowName, FARPG_WeaponData& OutWeaponData) const
{
    if (const FARPG_WeaponData* FoundWeaponData = WeaponDataTable->FindRow<FARPG_WeaponData>(*RowName, TEXT("")))
    {
        // 데이터를 출력 파라미터에 복사
        OutWeaponData = *FoundWeaponData;
        return true; // 성공
    }

    // 데이터를 찾지 못한 경우
    return false; // 실패
}

bool UARPG_GameInstance::TryGetCombatData(const FString& RowName, FARPG_CombatData& OutCombatData) const
{
    if (const FARPG_CombatData* FoundCombatData = CombatDataTable->FindRow<FARPG_CombatData>(*RowName, TEXT("")))
    {
        // 데이터를 출력 파라미터에 복사
        OutCombatData = *FoundCombatData;
        return true; // 성공
    }

    // 데이터를 찾지 못한 경우
    return false; // 실패
}

bool UARPG_GameInstance::TryGetCharacterData(const FString& RowName, FARPG_CharacterData& OutData) const
{
    if(CharacterDataTable == nullptr)
    {
        return false;
    }
    if (const FARPG_CharacterData* FoundData = CharacterDataTable->FindRow<FARPG_CharacterData>(*RowName, TEXT("")))
    {
        // 데이터를 출력 파라미터에 복사
        OutData = *FoundData;
        return true; // 성공
    }

    // 데이터를 찾지 못한 경우
    return false; // 실패
}