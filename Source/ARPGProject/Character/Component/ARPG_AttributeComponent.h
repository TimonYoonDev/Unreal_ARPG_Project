#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPG_AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_MULTICAST_DELEGATE(FOnStaminaChanged);
DECLARE_MULTICAST_DELEGATE(FOnExperienceChanged);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARPGPROJECT_API UARPG_AttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UARPG_AttributeComponent();

protected:
    virtual void BeginPlay() override;

public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Experience;

    FOnHealthChanged OnHealthChanged;

    FOnStaminaChanged OnStaminaChanged;

    FOnExperienceChanged OnExperienceChanged;

    FOnDeath OnDeath;


    void TakeDamage(float DamageAmount);

    void UseStamina(float StaminaAmount);

    void AddExperience(int32 Amount);

    float GetHealthPercent() const;

    float GetStaminaPercent() const;

    bool IsDeath() const;
};
