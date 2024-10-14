// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_BTT_Attack.h"

#include "ARPGProject/Character/ARPG_Character.h"
#include "ARPGProject/Character/MeleeCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UARPG_BTT_Attack::UARPG_BTT_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UARPG_BTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Result = EBTNodeResult::InProgress;
	if (AARPG_Character* Character = Cast<AARPG_Character>(OwnerActor); Character != nullptr)
	{
		IsAttacking = true;
		UKismetSystemLibrary::PrintString(GetWorld(), "Attack");
		Character->GetMeleeCombatComponent()->OnAttackEndDelegate.Remove(OnAttackEndDelegateHandle);
		OnAttackEndDelegateHandle = Character->GetMeleeCombatComponent()->OnAttackEndDelegate.AddUObject(this, &UARPG_BTT_Attack::OnAttackEnd);
		Character->GetMeleeCombatComponent()->InputAttack();

	}
	return Result;
}

void UARPG_BTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if(IsAttacking == false)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Attack Success");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


void UARPG_BTT_Attack::OnAttackEnd()
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Attack End");
	IsAttacking = false;
}
