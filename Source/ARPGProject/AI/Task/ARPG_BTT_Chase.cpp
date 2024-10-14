// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Chase.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"

EBTNodeResult::Type UARPG_BTT_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	if(AICharacterInterface == nullptr)
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();
		
		if (AIController == nullptr)
		{
			return Result;
		}

		AActor* Owner = AIController->GetPawn();
		if (Owner == nullptr)
		{
			return Result;
		}

		if (Owner->Implements<UARPG_AICharacterInterface>())
		{
			AICharacterInterface.SetObject(Owner);
			AICharacterInterface.SetInterface(Cast<IARPG_AICharacterInterface>(Owner));
			IARPG_AICharacterInterface::Execute_SetWalkSpeed(Owner, Speed);
			Result = EBTNodeResult::Succeeded;
		}

	}

	if(AICharacterInterface)
	{
		AICharacterInterface->Execute_SetWalkSpeed(AICharacterInterface.GetObject(), Speed);
		Result = EBTNodeResult::Succeeded;
	}
	return Result;
}
