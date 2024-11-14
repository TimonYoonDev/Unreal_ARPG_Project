// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Chase.h"

#include "ARPGProject/Character/ARPG_AICharacterInterface.h"
#include "ARPGProject/Character/ARPG_EnemyCharacter.h"

EBTNodeResult::Type UARPG_BTT_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if(AICharacterInterface)
	{
		if (AARPG_EnemyCharacter* Character = Cast<AARPG_EnemyCharacter>(OwnerActor); Character != nullptr)
		{
			Character->WeaponGrip();
		}

		AICharacterInterface->Execute_SetWalkSpeed(AICharacterInterface.GetObject(), Speed);
		Result = EBTNodeResult::Succeeded;
	}
	return Result;
}
