#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "ARPG_BTT_FocusTarget.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_BTT_FocusTarget : public UARPG_BTT_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
