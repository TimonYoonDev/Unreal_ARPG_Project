// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_AICharacterInterface.h"
#include "ARPG_Character.h"
#include "HealthBarWidget.h"
#include "ARPG_EnemyCharacter.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_EnemyCharacter : public AARPG_Character, public IARPG_AICharacterInterface
{
	GENERATED_BODY()
	AARPG_EnemyCharacter();

	static const FName LockOnPivotKey;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LockOnWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> LockOnWidget;

	virtual void BeginPlay() override;
	virtual void SetLockOnWidget(const bool bShowWidget);
	virtual void SetWalkSpeed_Implementation(const float InSpeed) override;

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void UpdateHealthBar();
	virtual void OnDeath() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;

	

};
