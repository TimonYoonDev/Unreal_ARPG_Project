// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PlayerController.h"

#include "ARPG_Character.h"
#include "ARPG_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

AARPG_PlayerController::AARPG_PlayerController()
{
	if(ConstructorHelpers::FClassFinder<UARPG_MainWidget>Widget (TEXT("/Game/ARPG/Blueprints/UI/WB_Main.WB_Main_C")); Widget.Succeeded())
	{
		HudWidgetClass = Widget.Class;
	}

	if (ConstructorHelpers::FClassFinder<UUserWidget>Widget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ARPG/Blueprints/UI/WB_Aim.WB_Aim_C'")); Widget.Succeeded())
	{
		AimWidgetClass = Widget.Class;
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerCharacter, &AARPG_PlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, PlayerCharacter, &AARPG_PlayerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputLightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputHeavyAttack);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputRoll);
		EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputGuard);
		EnhancedInputComponent->BindAction(TargetLockOnAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputTargetLockOn);
		EnhancedInputComponent->BindAction(ParkourAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputParkour);
		EnhancedInputComponent->BindAction(BowDrawAction, ETriggerEvent::Triggered, PlayerCharacter, &AARPG_PlayerCharacter::InputBowMode);


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
}
