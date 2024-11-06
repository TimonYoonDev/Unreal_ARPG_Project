// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AN_DeathEvent.h"

#include "ARPGProject/Character/ARPG_CharacterInterface.h"

void UARPG_AN_DeathEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(IARPG_CharacterInterface* CharacterInterface = Cast<IARPG_CharacterInterface>(MeshComp->GetOwner()))
	{
		CharacterInterface->FinishAttackDeath();
	}
}
