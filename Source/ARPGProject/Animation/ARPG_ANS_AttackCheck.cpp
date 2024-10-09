// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_ANS_AttackCheck.h"

#include "ARPGProject/Character/ARPG_CharacterInterface.h"

void UARPG_ANS_AttackCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	if (Owner->Implements<UARPG_CharacterInterface>())
	{
		IARPG_CharacterInterface::Execute_AttackCheckBegin(Owner);
	}
}

void UARPG_ANS_AttackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	if (Owner->Implements<UARPG_CharacterInterface>())
	{
		IARPG_CharacterInterface::Execute_AttackCheckEnd(Owner);
	}
}
