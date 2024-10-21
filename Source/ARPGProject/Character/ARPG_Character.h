// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_AIController.h"
#include "ARPG_CharacterInterface.h"
#include "ARPGProject/ARPG_WeaponBase.h"
#include "ARPGProject/DataTableStructs.h"
#include "GameFramework/Character.h"
#include "ARPGProject/ARPG_AttributeComponent.h"
#include "Components/WidgetComponent.h"

#include "ARPG_Character.generated.h"


class UARPG_AttributeComponent;
class UARPG_Attributes;
class UARPG_StatComponent;
class AARPG_PlayerState;
class UARPG_AnimInstance;
class AARPG_WeaponBase;
class UMeleeCombatComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UARPG_AttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AARPG_WeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AARPG_WeaponBase> WeaponBase;

	FRotator DirectionRotator;
	TObjectPtr<UParticleSystem> HitParticleSystem;

	AARPG_AIController* AIController;
	UWidgetComponent* WidgetComponent;
public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InputAttack();
	void HeavyAttackHold();
	void HeavyAttackCompleted();
	void InputWeaponChange(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputDefense(const FInputActionValue& Value);
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

protected:
	UFUNCTION()
	virtual void OnDeath();
private:

	TObjectPtr<AARPG_PlayerState> PlayerState;
	TObjectPtr<UARPG_AnimInstance> AnimInstance;
	AARPG_WeaponBase* CreateWeapon(TSubclassOf<AARPG_WeaponBase> WeaponBase);
	TArray<TObjectPtr<AARPG_WeaponBase>> EquipWeaponArray;
	TArray<FARPG_CombatData> CombatDataArray;
	int CurrentWeaponIndex;

	bool bEquipping;
	bool bRolling;
	bool bDefending;

	

	virtual void WeaponEquip_Implementation(bool InEquipping) override;
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UMeleeCombatComponent* GetMeleeCombatComponent() const { return MeleeCombatComponent; }

};
