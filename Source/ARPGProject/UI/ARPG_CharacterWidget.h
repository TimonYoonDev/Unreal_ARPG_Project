// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPG_CharacterWidget.generated.h"

class UProgressBar;
class UARPG_StatComponent;
/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_CharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
	//UARPG_CharacterWidget();

public:
	void BindCharacterStat(const UARPG_StatComponent* StatComponent);

private:
	TObjectPtr<UProgressBar> HealthProgressBar;
};
