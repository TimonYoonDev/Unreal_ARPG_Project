#include "ARPG_TitleUIWidget.h"

#include "ARPGProject/ARPG_GameInstance.h"
#include "ARPGProject/SaveLoadManager.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UARPG_TitleUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NewGameStartButton->OnClicked.AddDynamic(this, &UARPG_TitleUIWidget::OnClickNewGameStartButton);
	ContinueButton->OnClicked.AddDynamic(this, &UARPG_TitleUIWidget::OnClickContinueButton);
	ExitButton->OnClicked.AddDynamic(this, &UARPG_TitleUIWidget::OnClickExitButton);
	if(GameInstance = Cast<UARPG_GameInstance>(GetWorld()->GetGameInstance()); GameInstance != nullptr)
	{
		if(const USaveLoadManager* SaveLoadManager = GameInstance->GetSaveLoadManager())
		{
			const bool DoesSaveGameExist = SaveLoadManager->DoesSaveGameExist();
			ContinueButton->SetIsEnabled(DoesSaveGameExist);
		}		
	}
}

void UARPG_TitleUIWidget::OnClickNewGameStartButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), "ARPG_Map1");
}

void UARPG_TitleUIWidget::OnClickContinueButton()
{
	
}

void UARPG_TitleUIWidget::OnClickExitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
