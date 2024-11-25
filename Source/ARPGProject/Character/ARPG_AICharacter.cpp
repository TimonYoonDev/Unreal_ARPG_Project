#include "ARPG_AICharacter.h"
#include "Components/CapsuleComponent.h"
#include "ARPGProject/Character/HealthBarWidget.h"

const FName AARPG_AICharacter::LockOnPivotKey(TEXT("LockOnPivot"));

AARPG_AICharacter::AARPG_AICharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName("NPC");
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

bool AARPG_AICharacter::IsLockOnTarget() const
{
	return bIsLockOnTarget;
}

void AARPG_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	SetWidget();
	
}

void AARPG_AICharacter::OnDeath()
{
	Super::OnDeath();
	SetLockOnWidget(false);
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetHiddenInGame(true);
	}
}

float AARPG_AICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageCauser)
	{
		UAISense_Damage::ReportDamageEvent(GetWorld(), this,Cast<AActor>(EventInstigator->GetPawn()),
			ResultDamage,GetActorLocation(),DamageCauser->GetActorLocation()
		);
	}
	UpdateHealthBar();
	return ResultDamage;
}

void AARPG_AICharacter::SetWalkSpeed(const float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}

void AARPG_AICharacter::SetDefence(const bool bIsDefence)
{
	if (bIsDefence)
	{
		GetMeleeCombatComponent()->Guard();
	}
	else
	{
		GetMeleeCombatComponent()->GuardComplete();
	}
}

void AARPG_AICharacter::SetLockOn(const bool bIsLockOn)
{
	bIsLockOnTarget = bIsLockOn;
	GetCharacterMovement()->bUseControllerDesiredRotation = bIsLockOn;
	GetCharacterMovement()->bOrientRotationToMovement = !bIsLockOn;
	//GetCharacterMovement()->MaxWalkSpeed = IsLockOnTarget() ? 250 : 500;
}

void AARPG_AICharacter::SetLockOnWidget(const bool bShowWidget)
{
	LockOnWidgetComponent->SetVisibility(bShowWidget);
}

void AARPG_AICharacter::SetWidget()
{
	if(GameInstance)
	{
		if (HealthWidgetComponent)
		{
			if(GameInstance->HealthBarWidgetClass)
			{
				HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), GameInstance->HealthBarWidgetClass);
			}

			if (HealthBarWidget)
			{
				HealthWidgetComponent->SetWidget(HealthBarWidget);
				UpdateHealthBar();
			}
		}

		if (LockOnWidgetComponent)
		{
			LockOnWidget = CreateWidget<UUserWidget>(GetWorld(), GameInstance->LockOnWidgetClass);

			if (LockOnWidget)
			{
				LockOnWidgetComponent->SetWidget(LockOnWidget);
				LockOnWidgetComponent->SetVisibility(false);
			}
		}
	}
}

void AARPG_AICharacter::UpdateHealthBar() const
{
	if (AttributeComponent && HealthBarWidget)
	{
		HealthBarWidget->UpdateHealthBar(AttributeComponent->GetHealthPercent());
	}
}


/*
 *일반 공격 첫타 자동타겟팅 두번째부터는 해당 방향으로 공격

타겟팅하고 이동시에만 바라보기

가만히 있을때는 안움직임



적 AI

공격 사거리에 들어오면 계속 공격

한번씩 좌우 이동
*/