#pragma once

#include "CoreMinimal.h"
#include "ARPG_Character.h"
#include "ARPG_PlayerCharacter.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_PlayerCharacter : public AARPG_Character
{
	GENERATED_BODY()
	AARPG_PlayerCharacter();

public:
	bool bIsBowMode = false;
	bool bIsBowDrawing = false;
	float BowAimingPitch;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UARPG_CameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ArrowPos;

	float PressBowDrawingPower;
	const float PressBowDrawingMaxPower = 3.f;


	TSubclassOf<AARPG_WeaponBase> BowWeaponClass;
	TSubclassOf<AARPG_Projectile> ArrowClass;
	UPROPERTY()
	TObjectPtr<AARPG_WeaponBase> BowWeapon;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InputLightAttack(const FInputActionValue& Value);
	void InputHeavyAttack(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputGuard(const FInputActionValue& Value);
	void InputTargetLockOn(const FInputActionValue& Value);
	void InputParkour(const FInputActionValue& Value);
	void InputBowMode(const FInputActionValue& Value);


	


private:
	FVector GetAimLocation() const;
	void ShootArrow();
	void PressBowDrawing();

	void ParkourScanner();
	void ParkourScannerSub(FHitResult HitResult);
	void InteractWithObject();
};
