#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_BTT_Attack.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_BTT_Attack : public UARPG_BTT_Base
{
	GENERATED_BODY()

	FDelegateHandle OnMontageCancelDelegateHandle;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
