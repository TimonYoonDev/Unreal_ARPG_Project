#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPG_AttributeComponent.generated.h"

// 체력, 스태미나, 경험치 변화에 대한 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExperienceChanged);
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
    // 체력, 스태미나, 경험치 관련 속성
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

    // 체력과 스태미나 관련 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStaminaChanged OnStaminaChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnExperienceChanged OnExperienceChanged;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDeath OnDeath;
    // 체력 감소 처리

	UFUNCTION(BlueprintCallable, Category = "Attributes")
    void TakeDamage(float DamageAmount);

    // 스태미나 소모 처리
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void UseStamina(float StaminaAmount);

    // 경험치 추가 처리
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void AddExperience(int32 Amount);

    // 체력 및 스태미나 백분율 계산
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetStaminaPercent() const;

    bool IsDeath() const;
};
