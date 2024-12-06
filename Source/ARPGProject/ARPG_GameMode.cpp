#include "ARPG_GameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AARPG_GameMode::StartSlowMotion(float SlowDownFactor, float Duration)
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowDownFactor);

    // 지정된 시간이 지난 후 슬로우모션 해제
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AARPG_GameMode::ResetTimeDilation, Duration, false);
}

void AARPG_GameMode::ResetTimeDilation()
{
    // 원래 속도로 복원
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void AARPG_GameMode::IncreaseKillCount()
{
    CurrentKillCount++;
    UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Kill Count %d"), CurrentKillCount));
    if(IsCompleteMission())
    {
        OnGameEndEvent.Broadcast(true);
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
