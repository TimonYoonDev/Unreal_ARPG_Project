// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_BTT_Chase.generated.h"

class IARPG_AICharacterInterface;
/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_BTT_Chase : public UARPG_BTT_Base
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed;
};
