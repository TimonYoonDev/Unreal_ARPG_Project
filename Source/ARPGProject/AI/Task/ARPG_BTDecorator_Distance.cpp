#include "ARPG_BTDecorator_Distance.h"

#include "ARPGProject/Character/ARPG_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool UARPG_BTDecorator_Distance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(BlackboardComponent == nullptr)
	{
		return false;
	}

	const AARPG_Character* Target = Cast<AARPG_Character>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if(Target == nullptr)
	{
		return false;
	}
	
	const float Distance = Target->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn());
	
	if(MinDistance > Distance || MaxDistance < Distance)
	{
		return false;
	}

	return true;
}
