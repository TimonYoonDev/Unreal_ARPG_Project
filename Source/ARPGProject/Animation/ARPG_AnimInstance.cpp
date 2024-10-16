// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AnimInstance.h"

#include "KismetAnimationLibrary.h"
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

	if (CharacterMovement == nullptr)
		return;

	Velocity = CharacterMovement->Velocity;
	GroundSpeed = Velocity.Size2D();

	ShouldMove = /*CharacterMovement->GetCurrentAcceleration() != FVector::Zero() &&*/ GroundSpeed > 3.f;
	IsFalling = CharacterMovement->IsFalling();

	DirectionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());
	IsDefending = Character->IsDefending();

	if(bEquipWeapon)
	{
		EquipTime -= DeltaSeconds;
		bEquipWeapon = EquipTime > 0;
	}

	if(bHit)
	{
		HitTime -= DeltaSeconds;
		bHit = HitTime > 0;
	}
}

void UARPG_AnimInstance::EquipWeaponTrigger()
{
	bEquipWeapon = true;
	EquipTime = 1.f;
}

void UARPG_AnimInstance::SetEquipWeaponIndex(const int InWeaponIndex)
{
	EquipWeaponIndex = InWeaponIndex;
}

void UARPG_AnimInstance::HitTrigger(const float InHitAngle)
{
	bHit = true;
	HitTime = 0.2f;
	HitAngle = InHitAngle;
}
