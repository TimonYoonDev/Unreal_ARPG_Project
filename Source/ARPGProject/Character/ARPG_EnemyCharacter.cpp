// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_EnemyCharacter.h"

#include "ARPG_AIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const FName AARPG_EnemyCharacter::LockOnPivotKey(TEXT("spine_05"));

AARPG_EnemyCharacter::AARPG_EnemyCharacter()
{
	AIControllerClass = AARPG_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetMesh());
	HealthWidgetComponent->SetRelativeLocation(FVector(0, 0, 180));
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(GetMesh(), LockOnPivotKey);
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetDrawSize(FVector2D(40, 40));
}

void AARPG_EnemyCharacter::SetWalkSpeed_Implementation(const float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}

void AARPG_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthWidgetComponent)
	{
		if (HealthBarWidgetClass)
		{
			HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
		}

		if (HealthBarWidget)
		{
			HealthWidgetComponent->SetWidget(HealthBarWidget);
			UpdateHealthBar();
		}
	}

	if (LockOnWidgetComponent)
	{
		if (LockOnWidgetClass)
		{
			LockOnWidget = CreateWidget<UUserWidget>(GetWorld(), LockOnWidgetClass);
		}

		if (LockOnWidget)
		{
			LockOnWidgetComponent->SetWidget(LockOnWidget);
			LockOnWidgetComponent->SetVisibility(false);
		}
	}
}

void AARPG_EnemyCharacter::SetLockOnWidget(const bool bShowWidget)
{
	LockOnWidgetComponent->SetVisibility(bShowWidget);
}

float AARPG_EnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	const float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	AARPG_EnemyCharacter::UpdateHealthBar();
	return ResultDamage;
}

void AARPG_EnemyCharacter::UpdateHealthBar()
{
	if (AttributeComponent && HealthBarWidget)
	{
		float HealthPercent = AttributeComponent->GetHealthPercent();
		HealthBarWidget->UpdateHealthBar(HealthPercent);
	}
	else
	{
		if (GetWorld())
		{
			UKismetSystemLibrary::PrintString(GetWorld());
		}
	}
}

void AARPG_EnemyCharacter::OnDeath()
{
	Super::OnDeath();
	SetLockOnWidget(false);
}
