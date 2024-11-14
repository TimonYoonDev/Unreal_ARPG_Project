#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ARPG_AnimInstance.generated.h"

class AARPG_Character;

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
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	bool ShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	bool IsFalling;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = true))
	float DirectionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = true))
	bool bIsMainWeaponGrip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, meta = (AllowPrivateAccess = true))
	bool bIsCurrentMainWeaponGrip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit, meta = (AllowPrivateAccess = true))
	bool bHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit, meta = (AllowPrivateAccess = true))
	float HitTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit, meta = (AllowPrivateAccess = true))
	float HitAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsDefending;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsLockOnTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsBowMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsBowDrawing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float BowAimingPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float BowAimingAlpha;
};
