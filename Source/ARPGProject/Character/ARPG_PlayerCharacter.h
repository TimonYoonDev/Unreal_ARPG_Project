#pragma once

#include "CoreMinimal.h"
#include "ARPG_Character.h"
#include "ARPG_PlayerCharacter.generated.h"

class UARPG_CameraComponent;

UCLASS()
class ARPGPROJECT_API AARPG_PlayerCharacter : public AARPG_Character
{
	GENERATED_BODY()
	AARPG_PlayerCharacter();

public:
	bool bIsBowAiming = false;
	bool bIsBowDrawing = false;
	float BowAimingPitch;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> FinishAttackCameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UARPG_CameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ArrowPos;

	float PressBowDrawingPower;
	const float PressBowDrawingMaxPower = 3.f;


	TSubclassOf<AARPG_WeaponBase> BowWeaponClass;
	TSubclassOf<AARPG_Projectile> ArrowClass;
	UPROPERTY()
	TObjectPtr<AARPG_WeaponBase> BowWeapon;
	UPROPERTY()
	TObjectPtr<AARPG_Projectile> ArrowProjectile;

	UPROPERTY()
	TObjectPtr<USoundBase> BowDrawSound;
	UPROPERTY()
	TObjectPtr<USoundBase> BowShootSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BowDrawAudio;

	bool IsPlayingRoll;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Jump() override;	

	void InputMove(const FInputActionValue& Value);
	void InputLook(const FInputActionValue& Value);
	void InputLightAttack(const FInputActionValue& Value);
	void InputHeavyAttack(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputGuard(const FInputActionValue& Value);
	void InputTargetLockOn(const FInputActionValue& Value);
	void InputParkour(const FInputActionValue& Value);
	void InputBowAiming(const FInputActionValue& Value);
	void InputCrouch(const FInputActionValue& Value);

protected:
	virtual bool CanJumpInternal_Implementation() const override;

	virtual void FinishAttack() override;
	virtual void Assassinate() override;

private:
	void CreateArrowProjectile();
	FVector GetAimLocation() const;
	void ShootArrow();

	void ParkourScanner();
	void ParkourScannerSub(FHitResult HitResult);
	void InteractWithObject();

	void MoveCamera(USceneComponent* InTarget, USceneComponent* InParent);
};
