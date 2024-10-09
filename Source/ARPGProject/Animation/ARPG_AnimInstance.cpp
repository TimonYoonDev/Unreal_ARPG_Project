// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AnimInstance.h"

#include "GameFramework/Character.h"

void UARPG_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ACharacter>(GetOwningActor());
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

	ShouldMove = CharacterMovement->GetCurrentAcceleration() != FVector::Zero() && GroundSpeed > 3.f;
	IsFalling = CharacterMovement->IsFalling();

	//Angle = UKismetAnimationLibrary::CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());


	if(bEquipWeapon)
	{
		EquipTime -= DeltaSeconds;
		bEquipWeapon = EquipTime > 0;
	}
}

void UARPG_AnimInstance::EquipWeaponTrigger()
{
	bEquipWeapon = true;
	EquipTime = 1.f;
}

void UARPG_AnimInstance::SetEquipWeaponIndex(int InWeaponIndex)
{
	EquipWeaponIndex = InWeaponIndex;
}
