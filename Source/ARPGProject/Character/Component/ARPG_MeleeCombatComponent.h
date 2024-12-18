#pragma once

#include "CoreMinimal.h"
#include "ARPGProject/DataTableStructs.h"
#include "Components/ActorComponent.h"
#include "ARPG_MeleeCombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMontageEndDelegate, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGPROJECT_API UARPG_MeleeCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARPG_MeleeCombatComponent();

	void SetMontageData(const FARPG_MontageData& InMontageData);
	void InputAttack();
	void HeavyAttack(const UAnimMontage* NewHeavyAttackMontage);
	void HeavyAttackComplete(const UAnimMontage* NewHeavyAttackMontage);
	void SetNextCombo(const UAnimMontage* NewNextComboMontage);
	void SetStartComboMontage(UAnimMontage* InMontage);

	void Guard();
	void GuardComplete();
	bool IsGuard() const;

	void ParryStart();
	void ParryEnd();
	bool IsParry() const;
	bool bIsGuard;
	bool bIsParry;
	FTimerHandle ParryTimerHandle;
	float ParryTime = 0.5f;

	FOnMontageEndDelegate OnMontageEndDelegate;


protected:
	virtual void BeginPlay() override;

public:	
	void PlayMontage(const UAnimMontage* Montage, const FName NextSection = "");
	void StopMontage();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	bool IsMontagePlaying() const;

private:
	FARPG_MontageData MontageData;
	bool bMontagePlaying;
	bool bComboState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	UAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeleeCombo", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> StartComboMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeleeCombo", meta = (AllowPrivateAccess = "true"))
	const UAnimMontage* NextComboMontage;
};
