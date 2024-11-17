
#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "ARPG_BTT_Defence.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_BTT_Defence : public UARPG_BTT_Base
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	bool IsDefence;
};
