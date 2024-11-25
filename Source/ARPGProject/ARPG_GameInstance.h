// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableStructs.h"
#include "Engine/GameInstance.h"
#include "ARPG_GameInstance.generated.h"

class UHealthBarWidget;
class UNiagaraSystem;

UCLASS()
class ARPGPROJECT_API UARPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	UARPG_GameInstance();
public:
	UPROPERTY(EditAnywhere, Category = "Common")
	const TObjectPtr<UParticleSystem> HitParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Common")
	const TObjectPtr<UNiagaraSystem> GuardParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Common")
	const TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, Category = "Common Enemy")
	const TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Common Enemy")
	const TSubclassOf<UUserWidget> LockOnWidgetClass;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> WeaponDataTable;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> CombatDataTable;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> CharacterDataTable;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> MontageDataTable;

public:
	bool TryGetWeaponData(const FString& RowName, FARPG_WeaponData& OutWeaponData) const;
	bool TryGetCombatData(const FString& RowName, FARPG_CombatData& OutCombatData) const;
	bool TryGetCharacterData(const FString& RowName, FARPG_CharacterData& OutData) const;
	bool TryGetMontageData(const FString& RowName, FARPG_MontageData& OutData) const;
};
