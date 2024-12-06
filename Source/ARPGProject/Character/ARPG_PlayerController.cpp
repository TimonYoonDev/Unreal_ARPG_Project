// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PlayerController.h"

#include "ARPG_Character.h"
#include "ARPG_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ARPGProject/ARPG_GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AARPG_PlayerController::AARPG_PlayerController()
{
	if(ConstructorHelpers::FClassFinder<UARPG_MainWidget>Widget (TEXT("/Game/ARPG/Blueprints/UI/WB_MainUI.WB_MainUI_C")); Widget.Succeeded())
	{
		HudWidgetClass = Widget.Class;
	}

	if (ConstructorHelpers::FClassFinder<UUserWidget>Widget(TEXT("/Game/ARPG/Blueprints/UI/Character/WB_Aim.WB_Aim_C")); Widget.Succeeded())
	{
		AimWidgetClass = Widget.Class;
	}

	if (ConstructorHelpers::FClassFinder<UUserWidget>Widget(TEXT("/Game/ARPG/Blueprints/UI/WB_MenuUI.WB_MenuUI_C")); Widget.Succeeded())
	{
		MenuWidgetClass = Widget.Class;
	}

	if (ConstructorHelpers::FClassFinder<UUserWidget>Widget(TEXT("/Game/ARPG/Blueprints/UI/WB_CompleteUI.WB_CompleteUI_C")); Widget.Succeeded())
	{
		CompleteWidgetClass = Widget.Class;
	}

	if (ConstructorHelpers::FClassFinder<UUserWidget>Widget(TEXT("/Game/ARPG/Blueprints/UI/WB_GameOverUI.WB_GameOverUI_C")); Widget.Succeeded())
	{
		GameOverWidgetClass = Widget.Class;
	}
	
}

void AARPG_PlayerController::OnPossess(APawn* InPawn)
{

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("AARPG_PlayerController::OnPossess "));
	Super::OnPossess(InPawn);

	AARPG_PlayerCharacter* PlayerCharacter = Cast<AARPG_PlayerCharacter>(InPawn);
	if(PlayerCharacter == nullptr)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputLook);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerCharacter, &AARPG_PlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, PlayerCharacter, &AARPG_PlayerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputLightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputHeavyAttack);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputRoll);
		EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputGuard);
		EnhancedInputComponent->BindAction(TargetLockOnAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputTargetLockOn);
		EnhancedInputComponent->BindAction(ParkourAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputParkour);
		EnhancedInputComponent->BindAction(BowAimingAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputBowAiming);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputCrouch);
		EnhancedInputComponent->BindAction(EscAction, ETriggerEvent::Triggered, this, &AARPG_PlayerController::InputEsc);


	}

	if (HudWidgetClass)
	{
		MainWidget = CreateWidget<UARPG_MainWidget>(GetWorld(), HudWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport(); // 위젯을 화면에 추가
			MainWidget->SetBind(PlayerCharacter);
		}
	}

	if(AimWidgetClass)
	{
		AimWidget = CreateWidget<UUserWidget>(GetWorld(), AimWidgetClass);
		if(AimWidget)
		{
			AimWidget->AddToViewport();
			AimWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerCharacter->OnChangedBowAimMode.AddLambda([this](bool bIsAimMode) -> void
			{
				if(bIsAimMode)
				{
					AimWidget->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					AimWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			});
		}
	}

	if(MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
			MenuWidget->AddToViewport();
		}
	}

	if(AARPG_GameMode* GameMode = Cast<AARPG_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnGameEndEvent.AddUObject(this, &AARPG_PlayerController::OnGameEndEvent);
	}
}

void AARPG_PlayerController::InputEsc(const FInputActionValue& Value)
{
	if(Value.Get<bool>() == false)
	{
		if(MenuWidget->GetVisibility() == ESlateVisibility::Hidden)
		{
			MenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			MenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AARPG_PlayerController::OnGameEndEvent(bool IsComplete)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
	
	if(IsComplete)
	{
		if (CompleteWidgetClass)
		{
			CompleteWidget = CreateWidget<UUserWidget>(GetWorld(), CompleteWidgetClass);
			CompleteWidget->AddToViewport();
		}
	}
	else
	{
		if (GameOverWidgetClass)
		{
			GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
			GameOverWidget->AddToViewport();
		}
	}
}
