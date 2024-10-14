// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_BTT_Attack : public UARPG_BTT_Base
{
	GENERATED_BODY()
	UARPG_BTT_Attack();

	bool IsAttacking;
	FDelegateHandle OnAttackEndDelegateHandle;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	void OnAttackEnd();

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
