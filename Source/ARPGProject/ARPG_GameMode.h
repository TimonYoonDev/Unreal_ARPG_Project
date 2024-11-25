#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPG_GameMode.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void StartSlowMotion(float SlowDownFactor, float Duration);
	void ResetTimeDilation();
	
};
