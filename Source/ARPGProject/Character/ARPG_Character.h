#pragma once

#include "CoreMinimal.h"
#include "ARPG_AIController.h"
#include "ARPG_CharacterInterface.h"

#include "ARPGProject/ARPG_PlayerState.h"
#include "ARPGProject/ARPG_WeaponBase.h"
#include "ARPGProject/DataTableStructs.h"
#include "ARPGProject/Animation/ARPG_AnimInstance.h"

#include "Component/ARPG_AttributeComponent.h"
#include "Component/ARPG_LockOnSystemComponent.h"
#include "Component/MeleeCombatComponent.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"

#include "ARPG_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class ARPGPROJECT_API AARPG_Character : public ACharacter, public IARPG_CharacterInterface
{
	GENERATED_BODY()
public:
	AARPG_Character();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UMeleeCombatComponent* MeleeCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UARPG_AttributeComponent* AttributeComponent;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UARPG_LockOnSystemComponent* LockOnSystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AARPG_WeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AARPG_WeaponBase> WeaponBase;

	FRotator DirectionRotator;
	TObjectPtr<UParticleSystem> HitParticleSystem;

	AARPG_AIController* AIController;
	UWidgetComponent* HealthWidgetComponent;
public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InputAttack();
	void HeavyAttackStart();
	void HeavyAttackCompleted();
	void InputWeaponChange(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputDefense(const FInputActionValue& Value);
	void InputTargetLockOn(const FInputActionValue& Value);
	void SetWeapon(int NextWeaponIndex);

	bool IsRolling() const;
	bool IsDefending() const;

	virtual void SetNextCombo_Implementation(const UAnimMontage* NewNextComboMontage) override;
	virtual void AttackCheckBegin_Implementation() override;
	virtual void AttackCheckEnd_Implementation() override;
	virtual void WeaponAttach_Implementation(const FName AttachSocketName) override;


	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaSeconds) override;
protected:
	UFUNCTION()
	virtual void OnDeath();

	
private:
	
	TObjectPtr<AARPG_PlayerState> PlayerState;
	UPROPERTY()
	TObjectPtr<UARPG_AnimInstance> AnimInstance;

	AARPG_WeaponBase* CreateWeapon(TSubclassOf<AARPG_WeaponBase> WeaponBase);
	TArray<TObjectPtr<AARPG_WeaponBase>> EquipWeaponArray;
	TArray<FARPG_CombatData> CombatDataArray;

	int CurrentWeaponIndex;

	bool bEquipping;
	bool bRolling;
	bool bDefending;
	bool bTargetLockOn;

	

	virtual void WeaponEquip_Implementation(bool InEquipping) override;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UMeleeCombatComponent* GetMeleeCombatComponent() const { return MeleeCombatComponent; }
	FORCEINLINE UARPG_AttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
	FORCEINLINE UARPG_LockOnSystemComponent* GetLockOnSystemComponent() const { return LockOnSystemComponent; }

};
