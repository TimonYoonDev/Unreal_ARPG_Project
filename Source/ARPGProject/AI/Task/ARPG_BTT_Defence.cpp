// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Defence.h"

#include "ARPGProject/Character/ARPG_AICharacterInterface.h"

EBTNodeResult::Type UARPG_BTT_Defence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if(IARPG_AICharacterInterface* Interface = Cast<IARPG_AICharacterInterface>(ControlledPawn))
	{
		Interface->SetDefence(IsDefence);
	}

	return Result;
}
