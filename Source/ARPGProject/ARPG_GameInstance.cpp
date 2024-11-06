

#include "ARPG_GameInstance.h"



UARPG_GameInstance::UARPG_GameInstance()
{
	if(const ConstructorHelpers::FObjectFinder<UDataTable> DT_Data(TEXT("/Script/Engine.DataTable'/Game/ARPG/GameData/DT_WeaponData.DT_WeaponData'")); DT_Data.Succeeded())
	{
		WeaponDataTable = DT_Data.Object;
	}

	if(const ConstructorHelpers::FObjectFinder<UDataTable> DT_Data(TEXT("/Script/Engine.DataTable'/Game/ARPG/GameData/DT_CombatData.DT_CombatData'")); DT_Data.Succeeded())
	{
        CombatDataTable = DT_Data.Object;
	}

	if(const ConstructorHelpers::FObjectFinder<UDataTable> DT_Data(TEXT("/Script/Engine.DataTable'/Game/ARPG/GameData/DT_CharacterData.DT_CharacterData'")); DT_Data.Succeeded())
	{
        CharacterDataTable = DT_Data.Object;
	}

    if (const ConstructorHelpers::FObjectFinder<UDataTable> DT_Data(TEXT("/Script/Engine.DataTable'/Game/ARPG/GameData/DT_MontageData.DT_MontageData'")); DT_Data.Succeeded())
    {
        MontageDataTable = DT_Data.Object;
    }

}

bool UARPG_GameInstance::TryGetWeaponData(const FString& RowName, FARPG_WeaponData& OutWeaponData) const
{
    if (const FARPG_WeaponData* FoundWeaponData = WeaponDataTable->FindRow<FARPG_WeaponData>(*RowName, TEXT("")))
    {
        OutWeaponData = *FoundWeaponData;
        return true;
    }

    return false;
}

bool UARPG_GameInstance::TryGetCombatData(const FString& RowName, FARPG_CombatData& OutCombatData) const
{
    if (CombatDataTable == nullptr)
    {
        return false;
    }
    if (const FARPG_CombatData* FoundCombatData = CombatDataTable->FindRow<FARPG_CombatData>(*RowName, TEXT("")))
    {
        OutCombatData = *FoundCombatData;
        return true;
    }

    return false;
}

bool UARPG_GameInstance::TryGetCharacterData(const FString& RowName, FARPG_CharacterData& OutData) const
{
    if(CharacterDataTable == nullptr)
    {
        return false;
    }
    if (const FARPG_CharacterData* FoundData = CharacterDataTable->FindRow<FARPG_CharacterData>(*RowName, TEXT("")))
    {
        OutData = *FoundData;
        return true;
    }

    return false;
}

bool UARPG_GameInstance::TryGetMontageData(const FString& RowName, FARPG_MontageData& OutData) const
{
    if (MontageDataTable == nullptr)
    {
        return false;
    }
    if (const FARPG_MontageData* FoundData = MontageDataTable->FindRow<FARPG_MontageData>(*RowName, TEXT("")))
    {
        OutData = *FoundData;
        return true;
    }

    return false;
}
