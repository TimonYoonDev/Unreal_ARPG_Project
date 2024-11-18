#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include "DataTableStructs.generated.h"

USTRUCT(BlueprintType)
struct FARPG_WeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText WeaponName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AARPG_WeaponBase> WeaponClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AttackPower;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CombatDataRowName;
};

USTRUCT(BlueprintType)
struct FARPG_CombatData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CombatPoseIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> HeavyAttackMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bChargedAttack;

};

USTRUCT(BlueprintType)
struct FARPG_CharacterData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxHeath;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Damage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	//피격
	//리액션 시트
};

USTRUCT()
struct FARPG_MontageData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> ParryingReactionMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> GuardReactionMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> FinishAttackMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> FinishAttackReactionMontage;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> AssassinateMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> AssassinateReactionMontage;

};
