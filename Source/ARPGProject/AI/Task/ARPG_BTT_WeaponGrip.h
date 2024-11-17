// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "ARPG_BTT_WeaponGrip.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_BTT_WeaponGrip : public UARPG_BTT_Base
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
