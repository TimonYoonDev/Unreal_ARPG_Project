// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Base.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"

EBTNodeResult::Type UARPG_BTT_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	if (AICharacterInterface == nullptr)
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();

		if (AIController == nullptr)
		{
			return Result;
		}

		OwnerActor = AIController->GetPawn();
		if (OwnerActor == nullptr)
		{
			return Result;
		}

		if (OwnerActor->Implements<UARPG_AICharacterInterface>())
		{
			AICharacterInterface.SetObject(OwnerActor);
			AICharacterInterface.SetInterface(Cast<IARPG_AICharacterInterface>(OwnerActor));
			Result = EBTNodeResult::Succeeded;
		}

	}
	return Result;
}
