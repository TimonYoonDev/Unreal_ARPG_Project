// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

const FName AARPG_AIController::PatrolLocationKey(TEXT("PatrolLocation"));
const FName AARPG_AIController::TargetActorKey(TEXT("TargetActor"));
const FName AARPG_AIController::HasLineOfSightKey(TEXT("HasLineOfSight"));

AARPG_AIController::AARPG_AIController()
{
	const ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Enemy(TEXT("/Script/AIModule.BehaviorTree'/Game/ARPG/AI/BT_EnemyBase.BT_EnemyBase'"));
	
	if(BT_Enemy.Succeeded())
	{
		BehaviorTree = BT_Enemy.Object;
	}

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AARPG_AIController::OnTargetPerceptionUpdated);

	
}

void AARPG_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BehaviorTree);	
}

void AARPG_AIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	
	if(Actor->ActorHasTag("Player") && Stimulus.WasSuccessfullySensed())
	{
		GetWorld()->GetTimerManager().ClearTimer(EnemyTime);
		GetBlackboardComponent()->SetValueAsBool(HasLineOfSightKey, true);
		GetBlackboardComponent()->SetValueAsObject(TargetActorKey, Actor);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(EnemyTime,this,&AARPG_AIController::OnCleanTarget,LineOfSightTimer,false);
	}
}

void AARPG_AIController::OnCleanTarget()
{
	GetBlackboardComponent()->SetValueAsBool(HasLineOfSightKey, false);
	GetBlackboardComponent()->SetValueAsObject(TargetActorKey, nullptr);
}


