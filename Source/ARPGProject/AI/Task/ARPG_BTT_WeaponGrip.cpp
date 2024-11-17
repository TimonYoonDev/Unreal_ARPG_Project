// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_WeaponGrip.h"

#include "ARPGProject/Character/ARPG_EnemyCharacter.h"

EBTNodeResult::Type UARPG_BTT_WeaponGrip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if(AARPG_EnemyCharacter* Character = Cast<AARPG_EnemyCharacter>(ControlledPawn))
	{
		Character->WeaponGrip();
	}
	return Result;
}
