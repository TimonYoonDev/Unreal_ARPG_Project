#include "ARPG_AN_DeathEvent.h"

#include "ARPGProject/Character/ARPG_CharacterInterface.h"

void UARPG_AN_DeathEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	if(IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(Owner))
	{
		Interface->FinishAttackDeath();
	}
}
