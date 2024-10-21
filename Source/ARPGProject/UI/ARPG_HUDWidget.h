// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPG_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    /*UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealthBar(float HealthPercent);*/

};
