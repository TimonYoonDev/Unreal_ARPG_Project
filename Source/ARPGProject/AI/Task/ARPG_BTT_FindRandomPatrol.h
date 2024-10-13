// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_BTT_FindRandomPatrol.generated.h"

class IARPG_AICharacterInterface;

UCLASS()
class ARPGPROJECT_API UARPG_BTT_FindRandomPatrol : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	TScriptInterface<IARPG_AICharacterInterface> AICharacterInterface;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float PatrolSpeed = 125;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float PatrolRadius = 1000;

};
