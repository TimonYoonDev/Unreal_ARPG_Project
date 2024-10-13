// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_FindRandomPatrol.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ARPGProject/Character/ARPG_AIController.h"

EBTNodeResult::Type UARPG_BTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (nullptr == AIController)
	{
		return Result;
	}
	AActor* Owner = AIController->GetPawn();
	if (nullptr == Owner)
	{
		return Result;
	}
	/*auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}*/

	if(AICharacterInterface.GetObject() == nullptr)
	{
		if(Owner->Implements<UARPG_AICharacterInterface>())
		{
			AICharacterInterface.SetObject(Owner);
			AICharacterInterface.SetInterface(Cast<IARPG_AICharacterInterface>(Owner));
		}
	}
	
	if(AICharacterInterface.GetObject())
	{
		AICharacterInterface.GetInterface()->Execute_SetWalkSpeed(AICharacterInterface.GetObject(), PatrolSpeed);

		FVector NextPatrolLocation;
		if(UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), Owner->GetActorLocation(), NextPatrolLocation, PatrolRadius))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AARPG_AIController::PatrolLocationKey, NextPatrolLocation);
			Result = EBTNodeResult::Succeeded;
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(AARPG_AIController::PatrolLocationKey, Owner->GetActorLocation());
			Result = EBTNodeResult::Succeeded;
		}
	}

	return Result;
}
