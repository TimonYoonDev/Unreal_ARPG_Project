#include "ARPG_GameMode.h"
#include "Kismet/GameplayStatics.h"

void AARPG_GameMode::StartSlowMotion(float SlowDownFactor, float Duration)
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowDownFactor);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AARPG_GameMode::ResetTimeDilation, Duration, false);
}

void AARPG_GameMode::ResetTimeDilation()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void AARPG_GameMode::IncreaseKillCount()
{
    CurrentKillCount++;
    if(IsCompleteMission())
    {
        //OnGameEndEvent.Broadcast(true);
    }
}

bool AARPG_GameMode::IsCompleteMission()
{
    return CurrentKillCount >= CompleteKillCount;
}

void AARPG_GameMode::PlayerDeath()
{
    OnGameEndEvent.Broadcast(false);
}
