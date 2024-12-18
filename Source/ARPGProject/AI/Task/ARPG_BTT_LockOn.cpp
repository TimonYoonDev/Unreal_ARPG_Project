// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_LockOn.h"

#include "ARPGProject/Character/ARPG_AICharacterInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UARPG_BTT_LockOn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	if(IsLockOn)
	{
		if (const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName)); !TargetActor)
		{
			return EBTNodeResult::Failed;
		}
		if(AICharacterInterface)
		{
			AICharacterInterface->SetLockOn(true);
		}
	}
	else
	{
		if (AICharacterInterface)
		{
			AICharacterInterface->SetLockOn(false);
		}
	}	

	return Result;
}
