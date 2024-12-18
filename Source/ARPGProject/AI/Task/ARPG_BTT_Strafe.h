#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "ARPG_BTT_Strafe.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_BTT_Strafe : public UARPG_BTT_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 250.f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
