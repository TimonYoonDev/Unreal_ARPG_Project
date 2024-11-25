#include "ARPG_BTT_MontagePlay.h"

#include "ARPGProject/Character/ARPG_Character.h"
#include "ARPGProject/Character/Component/ARPG_MeleeCombatComponent.h"

EBTNodeResult::Type UARPG_BTT_MontagePlay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (const AARPG_Character* Character = Cast<AARPG_Character>(ControlledPawn); Character != nullptr)
	{
		Character->GetMeleeCombatComponent()->OnMontageCancelDelegate.Remove(OnMontageCancelDelegateHandle);
		OnMontageCancelDelegateHandle = Character->GetMeleeCombatComponent()->OnMontageCancelDelegate.AddLambda([this]()->void
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		});
		Character->GetMeleeCombatComponent()->PlayMontage(Montage);
	}
	return EBTNodeResult::InProgress;
}
