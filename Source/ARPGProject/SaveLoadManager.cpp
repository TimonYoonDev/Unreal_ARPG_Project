#include "SaveLoadManager.h"
#include "ARPG_SaveGame.h"
#include "Kismet/GameplayStatics.h"


void USaveLoadManager::SaveGameData(const APlayerController* PlayerController) const
{
    UARPG_SaveGame* SaveGameInstance = Cast<UARPG_SaveGame>(UGameplayStatics::CreateSaveGameObject(UARPG_SaveGame::StaticClass()));

    if (SaveGameInstance && PlayerController)
    {
        // 플레이어 데이터 가져오기
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            SaveGameInstance->PlayerLevel = 10; // 예제: 레벨
            SaveGameInstance->PlayerHealth = 75.0f; // 예제: 체력
            SaveGameInstance->PlayerLocation = PlayerPawn->GetActorLocation();
            SaveGameInstance->PlayerRotation = PlayerPawn->GetActorRotation();
        }

        // 데이터를 파일로 저장
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);
        UE_LOG(LogTemp, Log, TEXT("Game Saved!"));
    }
}

void USaveLoadManager::LoadGameData(const APlayerController* PlayerController)
{
    if (DoesSaveGameExist())
    {
        UE_LOG(LogTemp, Warning, TEXT("No save file found!"));
        return;
    }

    UARPG_SaveGame* LoadedGame = Cast<UARPG_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

    if (LoadedGame && PlayerController)
    {
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            PlayerPawn->SetActorLocation(LoadedGame->PlayerLocation);
            PlayerPawn->SetActorRotation(LoadedGame->PlayerRotation);
        }

        UE_LOG(LogTemp, Log, TEXT("Game Loaded!"));
        UE_LOG(LogTemp, Log, TEXT("Player Level: %d"), LoadedGame->PlayerLevel);
        UE_LOG(LogTemp, Log, TEXT("Player Health: %f"), LoadedGame->PlayerHealth);
    }
}

bool USaveLoadManager::DoesSaveGameExist() const
{
    return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}


    
