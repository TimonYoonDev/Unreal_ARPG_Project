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

	if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(Owner); Interface)
	{
		Interface->AttackCheckBegin();
	}
}

void UARPG_ANS_AttackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(Owner); Interface)
	{
		Interface->AttackCheckEnd();
	}
}
