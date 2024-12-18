// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ARPG_AIController.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_AIController : public AAIController
{
	GENERATED_BODY()

public:
	static const FName PatrolLocationKey;
	static const FName TargetActorKey;
	static const FName HasLineOfSightKey;
	static const FName StartLocationKey;

	UPROPERTY(BlueprintReadOnly)
	AActor* AttackTarget;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	UAISenseConfig_Damage* DamageConfig;

	

	FTimerHandle EnemyTime;
	float LineOfSightTimer = 2.f;

public:
	AARPG_AIController();

	virtual void RunAI(const TObjectPtr<UBehaviorTree>& BehaviorTree);
	virtual void StopAI();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;	
	
protected:

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void OnCleanTarget();
};
