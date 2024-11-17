// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_LockOn.h"
#include "ARPGProject/Character/ARPG_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UARPG_BTT_LockOn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}
	if(const AARPG_Character* Character = Cast<AARPG_Character>(ControlledPawn); Character)
	{
		Character->GetLockOnSystemComponent()->SetTarget(TargetActor);
	}

	return Result;
}
