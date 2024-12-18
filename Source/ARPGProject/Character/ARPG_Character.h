#pragma once

#include "CoreMinimal.h"
#include "ARPG_AIController.h"
#include "ARPG_CharacterInterface.h"
#include "ARPGProject/ARPG_GameInstance.h"

#include "ARPGProject/ARPG_PlayerState.h"
#include "ARPGProject/ARPG_Projectile.h"
#include "ARPGProject/ARPG_WeaponBase.h"
#include "ARPGProject/DataTableStructs.h"
#include "ARPGProject/Animation/ARPG_AnimInstance.h"

#include "MotionWarpingComponent.h"
#include "Component/ARPG_AttributeComponent.h"
#include "Component/ARPG_LockOnSystemComponent.h"
#include "Component/ARPG_MeleeCombatComponent.h"
#include "GameFramework/Character.h"

#include "ARPG_Character.generated.h"

class AARPG_GameMode;
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


DECLARE_MULTICAST_DELEGATE_OneParam(OnChangedBowAimMode, bool bIsAimMode)

UCLASS()
class ARPGPROJECT_API AARPG_Character : public ACharacter, public IARPG_CharacterInterface
{
	GENERATED_BODY()
public:
	AARPG_Character();

	OnChangedBowAimMode OnChangedBowAimMode;
protected:

	FARPG_CharacterData CharacterData;
	FARPG_CombatData CombatData;
	FARPG_MontageData MontageData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UARPG_MeleeCombatComponent* MeleeCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UARPG_AttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UARPG_LockOnSystemComponent* LockOnSystemComponent;

	UPROPERTY()
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AARPG_WeaponBase> WeaponBase;

	FRotator DirectionRotator;

	UPROPERTY()
	AARPG_AIController* AIController;

	UPROPERTY()
	UARPG_GameInstance* GameInstance;

	UPROPERTY()
	AARPG_GameMode* GameMode;

	UPROPERTY()
	TObjectPtr<UAnimMontage> ParkourAnim;

	UPROPERTY()
	TObjectPtr<AARPG_WeaponBase> MainWeapon;



	UPROPERTY(VisibleAnywhere, Category = "Finish Attack")
	TObjectPtr<AARPG_AICharacter> FinishAttackTargetActor;

	UPROPERTY(VisibleAnywhere, Category = "Assassinate")
	TObjectPtr<AARPG_AICharacter> AssassinateTarget;

	bool bIsMainWeaponGrip;
	bool bRolling;
private:
	UPROPERTY(EditAnywhere)
	FName CharacterKey = "Quinn";
	
	bool bTargetLockOn;
	bool bIsKnockBack;
	bool bIsFinishAttack;


	TObjectPtr<AARPG_PlayerState> PlayerState;
	UPROPERTY()
	TObjectPtr<UARPG_AnimInstance> AnimInstance;



public:
	void SetCharacterKey(const FName InCharacterKey);	

	bool IsMainWeaponGrip() const;
	bool IsRolling() const;
	bool IsGuard() const;
	bool IsKnockBack() const;
	virtual bool IsLockOnTarget() const;

	virtual void SetNextCombo(const UAnimMontage* NewNextComboMontage) override;
	virtual void AttackCheckBegin() override;
	virtual void AttackCheckEnd() override;
	virtual void WeaponAttach(const FName AttachSocketName) override;
	virtual void ParryingReaction() override;
	virtual void FinishAttackReaction() override;
	virtual void FinishAttackDeath() override;
	virtual void AssassinateReaction() override;
	virtual void SetMotionWarping(const AActor* InTarget) override;
	virtual void SetMotionWarping(const FVector& TargetLocation, const FRotator& TargetRotation) override;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaSeconds) override;

	void SetFinishAttackTarget(AARPG_AICharacter* InFinishAttackTarget);
	void SetAssassinateTarget(AARPG_AICharacter* InAssassinateTarget);

protected:
	UFUNCTION()
	virtual void OnMontageEndCallBack(bool bInterrupted);

	UFUNCTION()
	virtual void OnDeath();

	virtual void FinishAttack();
	virtual void Assassinate();

	AARPG_WeaponBase* CreateWeapon(const TSubclassOf<AARPG_WeaponBase>& InWeaponBase);


public:
	FORCEINLINE UARPG_MeleeCombatComponent* GetMeleeCombatComponent() const { return MeleeCombatComponent; }
	FORCEINLINE UARPG_AttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
	FORCEINLINE UARPG_LockOnSystemComponent* GetLockOnSystemComponent() const { return LockOnSystemComponent; }

};
