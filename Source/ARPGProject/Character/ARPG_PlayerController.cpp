// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PlayerController.h"

#include "ARPG_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AARPG_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AARPG_Character* ARPG_Character = Cast<AARPG_Character>(InPawn);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, ARPG_Character, &AARPG_Character::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, ARPG_Character, &AARPG_Character::StopJumping);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, ARPG_Character, &AARPG_Character::InputAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, ARPG_Character, &AARPG_Character::HeavyAttackHold);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Completed, ARPG_Character, &AARPG_Character::HeavyAttackCompleted);
		EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Started, ARPG_Character, &AARPG_Character::WeaponChange);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::InputRoll);
		
	}
}
