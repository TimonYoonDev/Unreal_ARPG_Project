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

#include "Component/ARPG_AttributeComponent.h"
#include "Component/ARPG_LockOnSystemComponent.h"
#include "Component/ARPG_MeleeCombatComponent.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "ARPG_Character.generated.h"

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
public:

	

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AARPG_WeaponBase> WeaponBase;

	FRotator DirectionRotator;
	UPROPERTY()
	UParticleSystem* HitParticleSystem;
	UPROPERTY()
	TObjectPtr<UNiagaraSystem> ParryParticleSystem;

	UPROPERTY()
	AARPG_AIController* AIController;
	UPROPERTY()
	UWidgetComponent* HealthWidgetComponent;
	UPROPERTY()
	UARPG_GameInstance* GameInstance;

	UPROPERTY()
	TObjectPtr<UAnimMontage> ParkourAnim;

	UPROPERTY()
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY()
	TObjectPtr<AARPG_WeaponBase> MainWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Finish Attack")
	TObjectPtr<AActor> FinishAttackTargetActor;

	bool bIsMainWeaponGrip;
	bool bCanFinishAttack;
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

	UPROPERTY(VisibleAnywhere, Category = "Finish Attack")
	USphereComponent* FinishAttackCollider;

	UPROPERTY(VisibleAnywhere, Category = "Assassinate")
	USphereComponent* AssassinateCollider;

public:
	void SetCharacterKey(const FName InCharacterKey);	

	bool IsMainWeaponGrip() const;

	bool IsRolling() const;
	bool IsGuard() const;
	bool IsKnockBack() const;

	virtual void SetNextCombo_Implementation(const UAnimMontage* NewNextComboMontage) override;
	virtual void AttackCheckBegin_Implementation() override;
	virtual void AttackCheckEnd_Implementation() override;
	virtual void WeaponAttach(const FName AttachSocketName) override;
	virtual void ParryingReaction() override;
	virtual void FinishAttack() override;
	virtual void FinishAttackDeath() override;


	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaSeconds) override;



	void SetCanFinishAttack(bool InCanFinishAttack, AActor* InFinishAttackTarget);
protected:
	UFUNCTION()
	virtual void OnDeath();

	AARPG_WeaponBase* CreateWeapon(const TSubclassOf<AARPG_WeaponBase>& InWeaponBase);
	
	
private:

	

	UFUNCTION()
	void OnFinishAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnFinishAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	

	
	
	

	
	

public:
	FORCEINLINE UARPG_MeleeCombatComponent* GetMeleeCombatComponent() const { return MeleeCombatComponent; }
	FORCEINLINE UARPG_AttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
	FORCEINLINE UARPG_LockOnSystemComponent* GetLockOnSystemComponent() const { return LockOnSystemComponent; }

};
