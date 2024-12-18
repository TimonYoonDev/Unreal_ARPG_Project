#include "ARPG_BTT_Attack.h"

#include "ARPGProject/Character/ARPG_Character.h"
#include "ARPGProject/Character/Component/ARPG_MeleeCombatComponent.h"

EBTNodeResult::Type UARPG_BTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (const AARPG_Character* Character = Cast<AARPG_Character>(ControlledPawn); Character != nullptr)
	{
		UARPG_MeleeCombatComponent* MeleeCombatComponent = Character->GetMeleeCombatComponent();
		if(MeleeCombatComponent == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		MeleeCombatComponent->OnMontageEndDelegate.Remove(OnMontageCancelDelegateHandle);
		OnMontageCancelDelegateHandle = MeleeCombatComponent->OnMontageEndDelegate.AddLambda([this](bool test)->void
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		});
		MeleeCombatComponent->InputAttack();
	}
	return EBTNodeResult::InProgress;
}
