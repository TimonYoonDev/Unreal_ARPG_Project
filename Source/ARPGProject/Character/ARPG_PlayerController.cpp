// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PlayerController.h"

#include "ARPG_Character.h"
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
	
}

void AARPG_PlayerController::OnPossess(APawn* InPawn)
{

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("AARPG_PlayerController::OnPossess "));
	Super::OnPossess(InPawn);

	AARPG_Character* ARPG_Character = Cast<AARPG_Character>(InPawn);
	if(ARPG_Character == nullptr)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, ARPG_Character, &AARPG_Character::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, ARPG_Character, &AARPG_Character::StopJumping);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, ARPG_Character, &AARPG_Character::InputAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, ARPG_Character, &AARPG_Character::HeavyAttackStart);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Completed, ARPG_Character, &AARPG_Character::HeavyAttackCompleted);
		EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::InputWeaponChange);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::InputRoll);
		EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::InputDefense);
		EnhancedInputComponent->BindAction(TargetLockOnAction, ETriggerEvent::Triggered, ARPG_Character, &AARPG_Character::InputTargetLockOn);
	}

	if (HudWidgetClass)
	{
		MainWidget = CreateWidget<UARPG_MainWidget>(GetWorld(), HudWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport(); // 위젯을 화면에 추가
			MainWidget->SetBind(ARPG_Character);
		}
	}
}
