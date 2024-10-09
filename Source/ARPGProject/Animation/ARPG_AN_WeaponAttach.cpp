// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AN_WeaponAttach.h"

#include "ARPGProject/Character/ARPG_CharacterInterface.h"

void UARPG_AN_WeaponAttach::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	if(Owner->Implements<UARPG_CharacterInterface>())
	{
		IARPG_CharacterInterface::Execute_WeaponAttach(Owner, AttachSocketName);
	}
}
