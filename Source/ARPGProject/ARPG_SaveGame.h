#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ARPG_SaveGame.generated.h"

UCLASS()
class ARPGPROJECT_API UARPG_SaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    int32 PlayerLevel;

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    float PlayerHealth;

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FVector PlayerLocation;

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FRotator PlayerRotation;
};

