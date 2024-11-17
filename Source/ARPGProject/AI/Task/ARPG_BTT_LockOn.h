// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "ARPG_BTT_LockOn.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_BTT_LockOn : public UARPG_BTT_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector TargetActorKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
