// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AIController.h"

#include "ARPGProject/ARPG_GameInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

const FName AARPG_AIController::PatrolLocationKey(TEXT("PatrolLocation"));
const FName AARPG_AIController::TargetActorKey(TEXT("TargetActor"));
const FName AARPG_AIController::HasLineOfSightKey(TEXT("HasLineOfSight"));

AARPG_AIController::AARPG_AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 감지 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f; // 시야 반경 설정
	SightConfig->LoseSightRadius = 1200.0f; // 시야에서 벗어난 거리 설정
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; // 시야각 설정
	//SightConfig->SetMaxAge(5.0f); // 감지 유지 시간
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// 데미지 감지 설정
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	//DamageConfig->SetMaxAge(2.0f); // 데미지 감지 유지 시간

	// AIPerceptionComponent에 설정 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 콜백 함수 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AARPG_AIController::OnTargetPerceptionUpdated);
}

void AARPG_AIController::RunAI(const TObjectPtr<UBehaviorTree>& BehaviorTree)
{
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
void AARPG_AIController::StopAI() const
{
	if(BrainComponent == nullptr)
	{
		return;
	}

	if (UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AARPG_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void AARPG_AIController::OnPossess(APawn* InPawn)
{
	
	Super::OnPossess(InPawn);
}

void AARPG_AIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(Actor->ActorHasTag("Player") == false)
	{
		return;
	}
	if (Stimulus.WasSuccessfullySensed())
	{
		GetWorld()->GetTimerManager().ClearTimer(EnemyTime);
		if (Stimulus.Type == SightConfig->GetSenseID())
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Sighted Target: %s"), *Actor->GetName()));
			// 타겟이 시야에 들어왔을 때의 로직
			GetBlackboardComponent()->SetValueAsBool(HasLineOfSightKey, true);
			GetBlackboardComponent()->SetValueAsObject(TargetActorKey, Actor);
			AttackTarget = Actor;
			
		}
		else if (Stimulus.Type == DamageConfig->GetSenseID())
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Damaged by Target: %s"), *Actor->GetName()));
			// 데미지를 입힌 타겟의 로직
			GetBlackboardComponent()->SetValueAsBool(HasLineOfSightKey, true);
			GetBlackboardComponent()->SetValueAsObject(TargetActorKey, Actor);
			AttackTarget = Actor;
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(EnemyTime, this, &AARPG_AIController::OnCleanTarget, LineOfSightTimer, false);
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Lost sight of: %s"), *Actor->GetName()));
	}
}

void AARPG_AIController::OnCleanTarget()
{
	GetBlackboardComponent()->SetValueAsBool(HasLineOfSightKey, false);
	GetBlackboardComponent()->SetValueAsObject(TargetActorKey, nullptr);
	AttackTarget = nullptr;
}


