#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "ARPG_BTT_Base.generated.h"

class IARPG_AICharacterInterface;

UCLASS()
class ARPGPROJECT_API UARPG_BTT_Base : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TObjectPtr<AActor> ControlledPawn;
	TObjectPtr<AAIController> OwnerController;

	IARPG_AICharacterInterface* AICharacterInterface;
};
