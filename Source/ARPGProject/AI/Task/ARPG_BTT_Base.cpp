// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Base.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"

EBTNodeResult::Type UARPG_BTT_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;

	OwnerController = OwnerComp.GetAIOwner();

	if (OwnerController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControlledPawn = OwnerController->GetPawn();
	if (ControlledPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if(AICharacterInterface)
	{
		return EBTNodeResult::Succeeded;
	}

	if (ControlledPawn->Implements<UARPG_AICharacterInterface>())
	{
		AICharacterInterface = Cast<IARPG_AICharacterInterface>(ControlledPawn);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
