#pragma once

#include "CoreMinimal.h"
#include "ARPG_AICharacterInterface.h"
#include "ARPG_Character.h"
#include "ARPG_AICharacter.generated.h"

class UHealthBarWidget;

UCLASS()
class ARPGPROJECT_API AARPG_AICharacter : public AARPG_Character, public IARPG_AICharacterInterface
{
	GENERATED_BODY()

	static const FName LockOnPivotKey;

	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> LockOnWidget;

	UPROPERTY()
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;
	UPROPERTY()
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;

	bool bIsLockOnTarget = false;

public:
	AARPG_AICharacter();

	virtual bool IsLockOnTarget() const override;
	virtual void SetLockOnWidget(const bool bShowWidget) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
private:

	virtual void SetWalkSpeed(const float InSpeed) override;
	virtual void SetDefence(const bool bIsDefence) override;
	virtual void SetLockOn(const bool bIsLockOn) override;
	

	void SetWidget();
	void UpdateHealthBar() const;
};
