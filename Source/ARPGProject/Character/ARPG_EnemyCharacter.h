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
	virtual void SetWalkSpeed_Implementation(const float InSpeed) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY()
	UHealthBarWidget* HealthBarWidget;

	virtual void BeginPlay() override;
protected:
	
	
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	// 체력 게이지 업데이트
	virtual void UpdateHealthBar();
	virtual void OnDeath() override;
	
};
