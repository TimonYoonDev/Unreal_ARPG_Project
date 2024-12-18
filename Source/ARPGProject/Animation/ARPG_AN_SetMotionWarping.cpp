// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AN_SetMotionWarping.h"

#include "ARPGProject/Character/ARPG_AIController.h"
#include "ARPGProject/Character/ARPG_CharacterInterface.h"

void UARPG_AN_SetMotionWarping::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(Owner))
	{
		if (const AARPG_AIController* AIController = Cast<AARPG_AIController>(Owner->GetInstigatorController()))
		{
			if(AIController == nullptr || AIController->AttackTarget == nullptr)
			{
				return;
			}
			Interface->SetMotionWarping(AIController->AttackTarget);
		}
		
	}
	
}
