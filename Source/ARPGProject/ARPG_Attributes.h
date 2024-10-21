// ARPG_Attributes.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_Attributes.generated.h"

UCLASS(Blueprintable)
class ARPGPROJECT_API UARPG_Attributes : public UObject
{
    GENERATED_BODY()

public:
    // 체력, 스테미나, 경험치, 상태 이상 등 공통 속성
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
    float Health;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
    float MaxHealth;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
    float Stamina;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
    float MaxStamina;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
    float Experience;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
    TArray<FString> StatusEffects;  // 상태 이상 (독, 스턴 등)

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetHealthPercent() const { return Health / MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetStaminaPercent() const { return Stamina / MaxStamina; }
};
