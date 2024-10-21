// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ARPG_AIController.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AARPG_AIController();

	void RunAI();
	void StopAI() const;

	virtual void OnPossess(APawn* InPawn) override;

	static const FName PatrolLocationKey;
	static const FName TargetActorKey;
	static const FName HasLineOfSightKey;
	
private:

	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	FTimerHandle EnemyTime;
	float LineOfSightTimer =2.f;


	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void OnCleanTarget();
};
