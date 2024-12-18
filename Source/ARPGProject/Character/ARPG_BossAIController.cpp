
#include "ARPG_BossAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

void AARPG_BossAIController::RunAI(const TObjectPtr<UBehaviorTree>& BehaviorTree)
{
	Super::RunAI(BehaviorTree);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	AttackTarget = PlayerController->GetPawn();
	GetBlackboardComponent()->SetValueAsBool(HasLineOfSightKey, true);
	GetBlackboardComponent()->SetValueAsObject(TargetActorKey, PlayerController->GetPawn());
	
}

void AARPG_BossAIController::StopAI()
{
	Super::StopAI();
	AttackTarget = nullptr;
	GetBlackboardComponent()->SetValueAsBool(HasLineOfSightKey, false);
	GetBlackboardComponent()->SetValueAsObject(TargetActorKey, nullptr);
}

void AARPG_BossAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//Super::OnTargetPerceptionUpdated(Actor, Stimulus);
}
