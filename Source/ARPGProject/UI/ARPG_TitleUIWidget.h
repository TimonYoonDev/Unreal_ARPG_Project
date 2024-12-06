#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPG_TitleUIWidget.generated.h"

class UARPG_GameInstance;
class UButton;

UCLASS()
class ARPGPROJECT_API UARPG_TitleUIWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* NewGameStartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY()
	UARPG_GameInstance* GameInstance;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickNewGameStartButton();
	UFUNCTION()
	void OnClickContinueButton();
	UFUNCTION()
	void OnClickExitButton();
};
