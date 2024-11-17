// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_FindRandomPatrol.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ARPGProject/Character/ARPG_AIController.h"

EBTNodeResult::Type UARPG_BTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	UE_LOG(LogTemp, Warning, TEXT("Task : %s / %s"), TEXT("ARPG_BTT_FindRandomPatrol"), *ControlledPawn->GetActorNameOrLabel());
	if(AICharacterInterface.GetObject())
	{
		AICharacterInterface.GetInterface()->SetWalkSpeed(PatrolSpeed);

		FVector NextPatrolLocation;
		if(UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), ControlledPawn->GetActorLocation(), NextPatrolLocation, PatrolRadius))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AARPG_AIController::PatrolLocationKey, NextPatrolLocation);
			Result = EBTNodeResult::Succeeded;
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AARPG_AIController::PatrolLocationKey, ControlledPawn->GetActorLocation());
			Result = EBTNodeResult::Succeeded;
		}
	}

	return Result;
}
