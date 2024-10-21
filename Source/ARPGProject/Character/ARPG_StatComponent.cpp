#include "ARPG_StatComponent.h"

#include "ARPGProject/ARPG_GameInstance.h"

UARPG_StatComponent::UARPG_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UARPG_StatComponent::SetDamage(const float NewDamage)
{
	SetHP(FMath::Clamp(CurrentHealth - NewDamage, 0, CharacterData.MaxHeath));
	
}

float UARPG_StatComponent::GetDamage() const
{
	return CharacterData.Damage;
}

void UARPG_StatComponent::SetHP(const float NewHP)
{
	CurrentHealth = NewHP;
	OnHPChangedDelegate.Broadcast();
	if (CurrentHealth < KINDA_SMALL_NUMBER)
	{
		CurrentHealth = 0;
		OnHPIsZeroDelegate.Broadcast();
	}
}

float UARPG_StatComponent::GetHPRatio() const
{
	return CurrentHealth < KINDA_SMALL_NUMBER ? 0 : CurrentHealth / CharacterData.MaxHeath;
}

void UARPG_StatComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UARPG_StatComponent::InitializeComponent()
{
	const UARPG_GameInstance* GameInstance = GetWorld()->GetGameInstance<UARPG_GameInstance>();
	const FString RowName("1");
	GameInstance->TryGetCharacterData(RowName, CharacterData);

}

