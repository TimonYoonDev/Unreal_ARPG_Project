// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_ANS_NextCombo.h"

#include "ARPGProject/Character/ARPG_CharacterInterface.h"

void UARPG_ANS_NextCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();
	if(Owner == nullptr)
	{
		return;
	}

	if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(Owner); Interface)
	{
		Interface->SetNextCombo(NextComboMontage);
	}
}

void UARPG_ANS_NextCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(Owner); Interface)
	{
		Interface->SetNextCombo(nullptr);
	}
}
