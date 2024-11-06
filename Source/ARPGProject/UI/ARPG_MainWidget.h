// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGProject/Character/ARPG_Character.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "ARPG_MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_MainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bind", meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bind", meta = (BindWidget))
    UProgressBar* StaminaBar;


    void SetBind(const AARPG_Character* Character);

    void UpdateHealthBar() const;
    void UpdateStaminaBar() const;

private:
    TWeakObjectPtr<UARPG_AttributeComponent> AttributeComponent;
};
