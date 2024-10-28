#include "ARPG_AttributeComponent.h"

UARPG_AttributeComponent::UARPG_AttributeComponent()
{
    // 기본 값 설정
    MaxHealth = 100.0f;
    Health = MaxHealth;

    MaxStamina = 100.0f;
    Stamina = MaxStamina;

    Experience = 0;

    // 컴포넌트가 Tick을 필요로 하지 않음
    PrimaryComponentTick.bCanEverTick = false;
}

void UARPG_AttributeComponent::BeginPlay()
{
    Super::BeginPlay();
}

// 체력 감소 처리
void UARPG_AttributeComponent::TakeDamage(float DamageAmount)
{
    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);

    // 체력 변화 델리게이트 브로드캐스트
    OnHealthChanged.Broadcast();

    if (IsDeath())
    {
        Health = 0.f;
        OnDeath.Broadcast();
    }
}

// 스태미나 소모 처리
void UARPG_AttributeComponent::UseStamina(float StaminaAmount)
{
    Stamina = FMath::Clamp(Stamina - StaminaAmount, 0.0f, MaxStamina);

    // 스태미나 변화 델리게이트 브로드캐스트
    OnStaminaChanged.Broadcast();
}

// 경험치 추가 처리
void UARPG_AttributeComponent::AddExperience(int32 Amount)
{
    Experience += Amount;

    // 경험치 변화 델리게이트 브로드캐스트
    OnExperienceChanged.Broadcast();
}

// 체력 백분율 계산
float UARPG_AttributeComponent::GetHealthPercent() const
{
    return MaxHealth > 0 ? Health / MaxHealth : 0.0f;
}

// 스태미나 백분율 계산
float UARPG_AttributeComponent::GetStaminaPercent() const
{
    return MaxStamina > 0 ? Stamina / MaxStamina : 0.0f;
}

bool UARPG_AttributeComponent::IsDeath() const
{
    return Health < KINDA_SMALL_NUMBER;
}
