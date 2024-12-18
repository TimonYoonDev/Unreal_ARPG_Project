// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "ARPGProject/Character/ARPG_Character.h"
#include "ARPGProject/Character/ARPG_PlayerCharacter.h"
#include "GameFramework/Character.h"

void UARPG_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AARPG_Character>(GetOwningActor());
	if (Character == nullptr)
		return;
	PlayerCharacter = Cast<AARPG_PlayerCharacter>(GetOwningActor());

	CharacterMovement = Character->GetCharacterMovement();
}

void UARPG_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(bExecuteNextFrame)
	{
		NativeLateUpdateAnimation();
	}
	else
	{
		bExecuteNextFrame = true;
	}

	if (CharacterMovement == nullptr)
		return;

	Velocity = CharacterMovement->Velocity;
	GroundSpeed = Velocity.Size2D();

	ShouldMove = GroundSpeed > 3.f;
	IsFalling = CharacterMovement->IsFalling();
	IsCrouching = CharacterMovement->IsCrouching();

	DirectionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());
	IsGuard = Character->IsGuard();
	IsLockOnTarget = Character->IsLockOnTarget();

	if (bHit)
	{
		HitTime -= DeltaSeconds;
		bHit = HitTime > 0;
	}

	bIsMainWeaponGrip = Character->IsMainWeaponGrip();

	if(PlayerCharacter)
	{
		IsAiming = PlayerCharacter->bIsBowAiming;
		IsDrawing = PlayerCharacter->bIsBowDrawing;
		AimingPitch = PlayerCharacter->GetBaseAimRotation().Pitch;
		AimingAlpha = IsAiming ? 1 : 0;
		IsStep = PlayerCharacter->IsRolling();
	}
	
}

void UARPG_AnimInstance::NativeLateUpdateAnimation()
{
	bExecuteNextFrame = false;
	bIsCurrentMainWeaponGrip = bIsMainWeaponGrip;
	IsCacheStep = IsStep;
}

void UARPG_AnimInstance::HitTrigger(const float InHitAngle)
{
	bHit = true;
	HitTime = 0.2f;
	HitAngle = InHitAngle;
}
