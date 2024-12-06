#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ARPG_AnimInstance.generated.h"

class AARPG_Character;
class AARPG_PlayerCharacter;

UCLASS()
class ARPGPROJECT_API UARPG_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	bool bExecuteNextFrame = false;
	void NativeLateUpdateAnimation();

public:
	void HitTrigger(float InHitAngle);


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	AARPG_Character* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	AARPG_PlayerCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	float GroundSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	bool ShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	bool IsFalling;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	float DirectionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	bool IsCrouching;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = true))
	bool bIsMainWeaponGrip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = true))
	bool bIsCurrentMainWeaponGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hit, meta = (AllowPrivateAccess = true))
	bool bHit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hit, meta = (AllowPrivateAccess = true))
	float HitTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hit, meta = (AllowPrivateAccess = true))
	float HitAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsGuard;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsLockOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Bow", meta = (AllowPrivateAccess = true))
	bool IsAiming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow", meta = (AllowPrivateAccess = true))
	bool IsDrawing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow", meta = (AllowPrivateAccess = true))
	float AimingPitch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow", meta = (AllowPrivateAccess = true))
	float AimingAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Step", meta = (AllowPrivateAccess = true))
	bool IsStep;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Step", meta = (AllowPrivateAccess = true))
	bool IsCacheStep;

	
};
