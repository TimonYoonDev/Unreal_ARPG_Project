// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const FName AARPG_AIController::PatrolLocationKey(TEXT("PatrolLocation"));
const FName AARPG_AIController::TargetActorKey(TEXT("TargetActor"));

AARPG_AIController::AARPG_AIController()
{
	const ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Enemy(TEXT("/Script/AIModule.BehaviorTree'/Game/ARPG/AI/BT_EnemyBase.BT_EnemyBase'"));
	
	if(BT_Enemy.Succeeded())
	{
		BehaviorTree = BT_Enemy.Object;
	}

	const ConstructorHelpers::FObjectFinder<UBlackboardData> BB_Enemy(TEXT("/Script/AIModule.BlackboardData'/Game/ARPG/AI/BB_EnemyBase.BB_EnemyBase'"));
	if (BB_Enemy.Succeeded())
	{
		BlackboardData = BB_Enemy.Object;
	}

	
}

void AARPG_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BlackboardData, BlackboardComp))
	{
		//AActor* test = BrainComponent->GetOwner();
		RunBehaviorTree(BehaviorTree);
	}
	
}

void AARPG_AIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

bool AARPG_AIController::RunBehaviorTree(UBehaviorTree* BTAsset)
{
	//return Super::RunBehaviorTree(BTAsset);
	// @todo: find BrainComponent and see if it's BehaviorTreeComponent
	// Also check if BTAsset requires BlackBoardComponent, and if so 
	// check if BB type is accepted by BTAsset.
	// Spawn BehaviorTreeComponent if none present. 
	// Spawn BlackBoardComponent if none present, but fail if one is present but is not of compatible class
	if (BTAsset == NULL)
	{
		UE_VLOG(this, LogBehaviorTree, Warning, TEXT("RunBehaviorTree: Unable to run NULL behavior tree"));
		return false;
	}

	bool bSuccess = true;

	// see if need a blackboard component at all
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (BTAsset->BlackboardAsset && (Blackboard == nullptr || Blackboard->IsCompatibleWith(BTAsset->BlackboardAsset) == false))
	{
		bSuccess = UseBlackboard(BTAsset->BlackboardAsset, BlackboardComp);
	}

	if (bSuccess)
	{
		UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (BTComp == NULL)
		{
			UE_VLOG(this, LogBehaviorTree, Log, TEXT("RunBehaviorTree: spawning BehaviorTreeComponent.."));

			BTComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BTComponent"));
			BTComp->RegisterComponent();
			REDIRECT_OBJECT_TO_VLOG(BTComp, this);
		}

		// make sure BrainComponent points at the newly created BT component
		BrainComponent = BTComp;
		AActor* dd = Cast<AAIController>(BrainComponent->GetOwner());
		UKismetSystemLibrary::PrintString(GetWorld(), dd->GetActorLabel());
		/*AAIController* dds = BrainComponent->GetAIOwner();
		UKismetSystemLibrary::PrintString(GetWorld(), dds->GetActorLabel());*/
		check(BTComp != NULL);
		BTComp->StartTree(*BTAsset, EBTExecutionMode::Looped);
	}

	return bSuccess;
}


