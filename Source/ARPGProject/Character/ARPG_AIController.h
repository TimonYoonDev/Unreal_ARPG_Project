// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARPG_AIController.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AARPG_AIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void PostInitializeComponents() override;
	virtual bool RunBehaviorTree(UBehaviorTree* BTAsset) override;

	static const FName PatrolLocationKey;
	static const FName TargetActorKey;
	
private:
	TObjectPtr<UBehaviorTree> BehaviorTree;
	TObjectPtr<UBlackboardData> BlackboardData;
	
};
