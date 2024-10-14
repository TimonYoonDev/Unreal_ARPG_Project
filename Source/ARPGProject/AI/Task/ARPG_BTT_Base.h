// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_BTT_Base.generated.h"

class IARPG_AICharacterInterface;
/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_BTT_Base : public UBTTaskNode
{
	GENERATED_BODY()

	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	TObjectPtr<AActor> OwnerActor;
	TScriptInterface<IARPG_AICharacterInterface> AICharacterInterface;
};
