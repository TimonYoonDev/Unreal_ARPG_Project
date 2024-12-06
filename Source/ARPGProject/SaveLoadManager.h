#pragma once

#include "CoreMinimal.h"
#include "SaveLoadManager.generated.h"


UCLASS()
class ARPGPROJECT_API USaveLoadManager : public UObject
{
    GENERATED_BODY()

public:
    void SaveGameData(const APlayerController* PlayerController) const;
    void LoadGameData(const APlayerController* PlayerController);
    bool DoesSaveGameExist() const;

private:
    FString SlotName = "PlayerSaveSlot";
    int32 UserIndex = 0;
};


