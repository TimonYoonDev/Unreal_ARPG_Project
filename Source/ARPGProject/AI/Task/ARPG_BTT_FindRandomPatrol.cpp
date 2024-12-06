// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_FindRandomPatrol.h"
#include "NavigationSystem.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ARPGProject/Character/ARPG_AIController.h"

EBTNodeResult::Type UARPG_BTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result == EBTNodeResult::Failed)
	{
		return Result;
	}

	if(AICharacterInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (FVector NextPatrolLocation; UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), AICharacterInterface->GetStartSpawnLocation(), NextPatrolLocation, PatrolRadius))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AARPG_AIController::PatrolLocationKey, NextPatrolLocation);
		Result = EBTNodeResult::Succeeded;
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AARPG_AIController::PatrolLocationKey, AICharacterInterface->GetStartSpawnLocation());
		Result = EBTNodeResult::Succeeded;
	}

	return Result;
}
