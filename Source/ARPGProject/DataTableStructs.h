#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include "DataTableStructs.generated.h"

USTRUCT(BlueprintType)
struct FARPG_WeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText WeaponName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AARPG_WeaponBase> WeaponClass;
};

USTRUCT(BlueprintType)
struct FARPG_CombatData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HeavyAttackMontage;

	UPROPERTY(EditAnywhere)
	bool bChargedAttack;

};

USTRUCT(BlueprintType)
struct FARPG_CharacterData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 MaxHeath;
	UPROPERTY(EditAnywhere)
	int32 Damage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AARPG_WeaponBase> WeaponClass;
};

USTRUCT()
struct FARPG_MontageData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ParryingReactionMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> GuardReactionMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FinishAttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FinishAttackReactionMontage;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AssassinateMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AssassinateReactionMontage;

};
