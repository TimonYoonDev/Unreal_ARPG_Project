#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPG_MeleeCombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGPROJECT_API UARPG_MeleeCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARPG_MeleeCombatComponent();

	void InputAttack();
	void HeavyAttack(const UAnimMontage* NewHeavyAttackMontage);
	void HeavyAttackComplete(const UAnimMontage* NewHeavyAttackMontage);
	void SetNextCombo(const UAnimMontage* NewNextComboMontage);
	void SetStartComboMontage(UAnimMontage* InMontage);

	FOnAttackEndDelegate OnAttackEndDelegate;

protected:
	virtual void BeginPlay() override;

public:	
	void PlayMontage(const UAnimMontage* Montage, const FName NextSection = "");

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	bool GetMontagePlaying() const;

private:
	bool bMontagePlaying;
	bool bComboState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	UAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeleeCombo", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> StartComboMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeleeCombo", meta = (AllowPrivateAccess = "true"))
	const UAnimMontage* NextComboMontage;
};
