#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ARPG_AN_SetMotionWarping.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_AN_SetMotionWarping : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
