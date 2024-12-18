// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Strafe.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"
#include "ARPGProject/Character/ARPG_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UARPG_BTT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if(Result == EBTNodeResult::Succeeded)
	{
		AICharacterInterface->SetWalkSpeed(WalkSpeed);
		AICharacterInterface->SetDefence(true);
		AICharacterInterface->SetLockOn(true);
		if(OwnerComp.GetBlackboardComponent())
		{
			AARPG_Character* Target = Cast<AARPG_Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
			OwnerController->SetFocus(Target);
		}
		
	}

	return Result;
}
