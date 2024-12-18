#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARPG_AIController.h"
#include "ARPG_BossAIController.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_BossAIController : public AARPG_AIController
{
	GENERATED_BODY()


public:
	virtual void RunAI(const TObjectPtr<UBehaviorTree>& BehaviorTree) override;
	virtual void StopAI() override;
protected:
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;
};
