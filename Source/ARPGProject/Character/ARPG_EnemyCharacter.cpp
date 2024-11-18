// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_EnemyCharacter.h"

#include "ARPG_AIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const FName AARPG_EnemyCharacter::LockOnPivotKey(TEXT("LockOnPivot"));

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

void AARPG_EnemyCharacter::WeaponGrip()
{
	bIsMainWeaponGrip = true;
	WeaponAttach("Sword_Grip");
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

void AARPG_EnemyCharacter::SetWalkSpeed(const float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}

void AARPG_EnemyCharacter::SetDefence(const bool bIsDefence)
{
	if(bIsDefence)
	{
		GetMeleeCombatComponent()->Guard();
	}
	else
	{
		GetMeleeCombatComponent()->GuardComplete();
	}
}

float AARPG_EnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                       AController* EventInstigator, AActor* DamageCauser)
{
	const float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	AARPG_EnemyCharacter::UpdateHealthBar();

	if (ResultDamage > 0.0f && DamageCauser)
	{
		// 데미지 이벤트 보고
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			this,                       // 데미지 받은 AI 캐릭터
			Cast<AActor>(EventInstigator->GetPawn()),               // 데미지를 입힌 Actor (플레이어 등)
			ResultDamage,               // 데미지 양
			GetActorLocation(),         // 데미지 받은 위치
			DamageCauser->GetActorLocation() // 데미지 가한 Actor 위치
		);

		// 데미지에 대한 추가 처리(예: 체력 감소) 등을 여기서 수행할 수 있습니다.
	}
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
