// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ARPG_BTDecorator_Distance.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_BTDecorator_Distance : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	float MinDistance;
	UPROPERTY(EditAnywhere)
	float MaxDistance;


	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
