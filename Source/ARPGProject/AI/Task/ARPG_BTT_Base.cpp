// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Base.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"

EBTNodeResult::Type UARPG_BTT_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	//if (AICharacterInterface == nullptr)
	{
		OwnerController = OwnerComp.GetAIOwner();

		if (OwnerController == nullptr)
		{
			return Result;
		}

		ControlledPawn = OwnerController->GetPawn();
		if (ControlledPawn == nullptr)
		{
			return Result;
		}

		/*UE_LOG(LogTemp, Warning, TEXT("Task : %s / %s"), *OwnerController->GetActorNameOrLabel(), *ControlledPawn->GetActorNameOrLabel());*/

		if (ControlledPawn->Implements<UARPG_AICharacterInterface>())
		{
			AICharacterInterface.SetObject(ControlledPawn);
			AICharacterInterface.SetInterface(Cast<IARPG_AICharacterInterface>(ControlledPawn));
			Result = EBTNodeResult::Succeeded;
		}

	}
	return Result;
}
