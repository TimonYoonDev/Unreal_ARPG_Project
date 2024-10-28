// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Chase.h"

#include "AIController.h"
#include "ARPGProject/Character/ARPG_AICharacterInterface.h"

EBTNodeResult::Type UARPG_BTT_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	UE_LOG(LogTemp, Warning, TEXT("Task : %s / %s"), TEXT("ARPG_BTT_Chase"), *OwnerActor->GetActorNameOrLabel());
	if(AICharacterInterface)
	{
		AICharacterInterface->Execute_SetWalkSpeed(AICharacterInterface.GetObject(), Speed);
		Result = EBTNodeResult::Succeeded;
	}
	return Result;
}
