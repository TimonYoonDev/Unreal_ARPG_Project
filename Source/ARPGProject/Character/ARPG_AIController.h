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
	AARPG_AIController();

	void RunAI();
	void StopAI() const;

	virtual void OnPossess(APawn* InPawn) override;

	static const FName PatrolLocationKey;
	static const FName TargetActorKey;
	static const FName HasLineOfSightKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	UAISenseConfig_Sight* SightConfig;
private:

	TObjectPtr<UBehaviorTree> BehaviorTree;
	

	

	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	UAISenseConfig_Damage* DamageConfig;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);


	FTimerHandle EnemyTime;
	float LineOfSightTimer =5.f;


	void OnCleanTarget();
};
