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
#include "Component/ARPG_CameraComponent.h"
#include "Component/ARPG_LockOnSystemComponent.h"
#include "Component/ARPG_MeleeCombatComponent.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName CharacterKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* HitReactionMontageData;

	OnChangedBowAimMode OnChangedBowAimMode;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UARPG_CameraComponent* CameraComponent;

	///

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
	UParticleSystem* ParryParticleSystem;

	UPROPERTY()
	AARPG_AIController* AIController;
	UPROPERTY()
	UWidgetComponent* HealthWidgetComponent;
	UPROPERTY()
	UARPG_GameInstance* GameInstance;

	UPROPERTY()
	TObjectPtr<UAnimMontage> ParkourAnim;

	UPROPERTY()
	USoundBase* DeathSound;
public:
	void SetCharacterKey(const FName InCharacterKey);
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InputAttack(const FInputActionValue& Value);
	void HeavyAttackStart();
	void HeavyAttackCompleted();
	void InputWeaponChange(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputDefense(const FInputActionValue& Value);
	void InputTargetLockOn(const FInputActionValue& Value);
	void InputParkour(const FInputActionValue& Value);
	void InputBowMode(const FInputActionValue& Value);
	bool bIsBowMode = false;
	bool bIsBowDrawing = false;

	bool IsMainWeaponGrip() const;
	float PressBowDrawingPower;
	const float PressBowDrawingMaxPower = 3.f;
	void PressBowDrawing();
	TSubclassOf<AARPG_WeaponBase> BowWeaponClass;
	UPROPERTY()
	TObjectPtr<AARPG_WeaponBase> BowWeapon;

	TSubclassOf<AARPG_Projectile> ArrowClass;
	UPROPERTY()
	USceneComponent* ArrowPos;

	FVector GetAimLocation() const;
	void ShootArrow();

	float BowAimingPitch;

	bool IsRolling() const;
	bool IsDefending() const;
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
	void ParkourScanner();
	void ParkourScannerSub(FHitResult HitResult);


	void SetCanFinishAttack(bool InCanFinishAttack, AActor* InFinishAttackTarget);
protected:
	UFUNCTION()
	virtual void OnDeath();
	bool bIsMainWeaponGrip;
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Finish Attack")
	USphereComponent* FinishAttackCollider;

	UPROPERTY(VisibleAnywhere, Category = "Assassinate")
	USphereComponent* AssassinateCollider;

	UFUNCTION()
	void OnFinishAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnFinishAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bCanFinishAttack;
	UPROPERTY(VisibleAnywhere, Category = "Finish Attack")
	AActor* FinishAttackTargetActor;
	
	TObjectPtr<AARPG_PlayerState> PlayerState;
	UPROPERTY()
	TObjectPtr<UARPG_AnimInstance> AnimInstance;

	AARPG_WeaponBase* CreateWeapon(const TSubclassOf<AARPG_WeaponBase>& InWeaponBase);
	UPROPERTY()
	TObjectPtr<AARPG_WeaponBase> MainWeapon;
	FARPG_CombatData CombatData;
	FARPG_MontageData MontageData;

	
	bool bRolling;
	bool bTargetLockOn;
	bool bIsKnockBack;
	bool bIsFinishAttack;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UARPG_MeleeCombatComponent* GetMeleeCombatComponent() const { return MeleeCombatComponent; }
	FORCEINLINE UARPG_AttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
	FORCEINLINE UARPG_LockOnSystemComponent* GetLockOnSystemComponent() const { return LockOnSystemComponent; }

};
