// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_ANS_WeaponEquip.h"

#include "ARPGProject/Character/ARPG_CharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UARPG_ANS_WeaponEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	if (Owner->Implements<UARPG_CharacterInterface>())
	{
		IARPG_CharacterInterface::Execute_WeaponEquip(Owner, true);
		UKismetSystemLibrary::PrintString(GetWorld(), "Equip Begin");
	}
}

void UARPG_ANS_WeaponEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	if (Owner->Implements<UARPG_CharacterInterface>())
	{
		IARPG_CharacterInterface::Execute_WeaponEquip(Owner, false);
		UKismetSystemLibrary::PrintString(GetWorld(),"Equip End");
	}
}
