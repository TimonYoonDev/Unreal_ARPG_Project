#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPG_GameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnGameEndEvent, bool);

UCLASS()
class ARPGPROJECT_API AARPG_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	OnGameEndEvent OnGameEndEvent;

private:
	int CompleteKillCount = 4;
	int CurrentKillCount = 0;

public:
	void StartSlowMotion(float SlowDownFactor, float Duration);
	void ResetTimeDilation();
	void IncreaseKillCount();
	bool IsCompleteMission();
	void PlayerDeath();
	
};
