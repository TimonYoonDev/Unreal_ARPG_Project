// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_EnemyCharacter.h"

#include "ARPG_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AARPG_EnemyCharacter::AARPG_EnemyCharacter()
{
	AIControllerClass = AARPG_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AARPG_EnemyCharacter::SetWalkSpeed_Implementation(const float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}
