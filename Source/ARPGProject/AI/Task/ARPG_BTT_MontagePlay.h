#pragma once

#include "CoreMinimal.h"
#include "ARPG_BTT_Base.h"
#include "ARPG_BTT_MontagePlay.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_BTT_MontagePlay : public UARPG_BTT_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;

	FDelegateHandle OnMontageCancelDelegateHandle;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
