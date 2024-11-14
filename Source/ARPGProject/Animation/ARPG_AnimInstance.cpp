// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AnimInstance.h"

#include <filesystem>

#include "KismetAnimationLibrary.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "ARPGProject/Character/ARPG_Character.h"
#include "GameFramework/Character.h"

void UARPG_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AARPG_Character>(GetOwningActor());
	if (Character == nullptr)
		return;

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

	ShouldMove = /*CharacterMovement->GetCurrentAcceleration() != FVector::Zero() &&*/ GroundSpeed > 3.f;
	IsFalling = CharacterMovement->IsFalling();

	DirectionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());
	IsDefending = Character->IsDefending();
	if (Character->GetLockOnSystemComponent())
	{
		IsLockOnTarget = Character->GetLockOnSystemComponent()->IsLockOnTarget();
	}

	if (bHit)
	{
		HitTime -= DeltaSeconds;
		bHit = HitTime > 0;
	}

	bIsMainWeaponGrip = Character->IsMainWeaponGrip();

	IsBowMode = Character->bIsBowMode;
	IsBowDrawing = Character->bIsBowDrawing;
	BowAimingPitch = Character->BowAimingPitch;
	BowAimingAlpha = IsBowMode ? 1 : 0;
}

void UARPG_AnimInstance::NativeLateUpdateAnimation()
{
	bExecuteNextFrame = false;
	bIsCurrentMainWeaponGrip = bIsMainWeaponGrip;
}

void UARPG_AnimInstance::HitTrigger(const float InHitAngle)
{
	bHit = true;
	HitTime = 0.2f;
	HitAngle = InHitAngle;
}
