#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "ARPG_BTT_LockOn.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_BTT_LockOn : public UARPG_BTT_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool IsLockOn;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
