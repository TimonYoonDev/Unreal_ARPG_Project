#include "ARPG_AICharacter.h"

#include "Components/CapsuleComponent.h"
#include "ARPGProject/Character/HealthBarWidget.h"
#include "Components/WidgetComponent.h"

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

	AssassinateWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AssassinateWidgetComponent"));
	AssassinateWidgetComponent->SetupAttachment(GetMesh());
	AssassinateWidgetComponent->SetRelativeLocation(FVector(0, 0, 200));
	AssassinateWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	// 전방에 위치할 FinishAttackCollider 초기화
	FinishAttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("FinishAttackCollider"));
	FinishAttackCollider->SetupAttachment(RootComponent);

	// 콜라이더 크기와 위치 설정
	FinishAttackCollider->SetSphereRadius(100.0f);  // 필요에 따라 범위를 조정
	FinishAttackCollider->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	FinishAttackCollider->SetGenerateOverlapEvents(false);

	// 오버랩 이벤트 바인딩
	FinishAttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AARPG_AICharacter::OnFinishAttackOverlapBegin);
	FinishAttackCollider->OnComponentEndOverlap.AddDynamic(this, &AARPG_AICharacter::OnFinishAttackOverlapEnd);

	// 후방에 위치할 AssassinateCollider 초기화
	AssassinateCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AssassinateCollider"));
	AssassinateCollider->SetupAttachment(RootComponent);

	// 콜라이더 크기와 위치 설정
	AssassinateCollider->SetSphereRadius(100.0f);  // 필요에 따라 범위를 조정
	AssassinateCollider->SetRelativeLocation(FVector(-100.0f, 0.0f, 0.0f));

	// 오버랩 이벤트 바인딩
	AssassinateCollider->OnComponentBeginOverlap.AddDynamic(this, &AARPG_AICharacter::OnAssassinateOverlapBegin);
	AssassinateCollider->OnComponentEndOverlap.AddDynamic(this, &AARPG_AICharacter::OnAssassinateOverlapEnd);
}

bool AARPG_AICharacter::IsLockOnTarget() const
{
	return bIsLockOnTarget;
}

void AARPG_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	SetWidget();
	StartSpawnLocation = GetActorLocation();
	
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
	HealthWidgetComponent->SetVisibility(true);
	UpdateHealthBar();
	return ResultDamage;
}

void AARPG_AICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(AttributeComponent->IsDeath())
	{
		return;
	}
	if(AIController)
	{
		AssassinateCollider->SetGenerateOverlapEvents(AIController->AttackTarget == nullptr);
	}

	
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
}

FVector AARPG_AICharacter::GetStartSpawnLocation()
{
	return StartSpawnLocation;
}

void AARPG_AICharacter::ParryingReaction()
{
	Super::ParryingReaction();
	FinishAttackCollider->SetGenerateOverlapEvents(true);
}

void AARPG_AICharacter::FinishAttackReaction()
{
	Super::FinishAttackReaction();
	AttributeComponent->TakeDamage(AttributeComponent->Health);
	HealthWidgetComponent->SetVisibility(false);
	AssassinateCollider->SetGenerateOverlapEvents(false);
	AssassinateWidgetComponent->SetVisibility(false);
	FinishAttackCollider->SetGenerateOverlapEvents(false);
}

void AARPG_AICharacter::AssassinateReaction()
{
	Super::AssassinateReaction();
	AttributeComponent->TakeDamage(AttributeComponent->Health);
	HealthWidgetComponent->SetVisibility(false);
	AssassinateCollider->SetGenerateOverlapEvents(false);
	AssassinateWidgetComponent->SetVisibility(false);
}

void AARPG_AICharacter::SetLockOnWidget(const bool bShowWidget)
{
	LockOnWidgetComponent->SetVisibility(bShowWidget);
}

void AARPG_AICharacter::SetAssassinateWidget(const bool bShowWidget)
{
	AssassinateWidgetComponent->SetVisibility(bShowWidget);
}

void AARPG_AICharacter::OnMontageEndCallBack(bool bInterrupted)
{
	Super::OnMontageEndCallBack(bInterrupted);
	FinishAttackCollider->SetGenerateOverlapEvents(false);
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
				HealthWidgetComponent->SetVisibility(false);
				UpdateHealthBar();
			}
		}

		if (LockOnWidgetComponent)
		{
			if (UUserWidget* LockOnWidget = CreateWidget<UUserWidget>(GetWorld(), GameInstance->LockOnWidgetClass))
			{
				LockOnWidgetComponent->SetWidget(LockOnWidget);
				LockOnWidgetComponent->SetVisibility(false);
			}
		}

		if (AssassinateWidgetComponent)
		{
			if (UUserWidget* AssassinateWidget = CreateWidget<UUserWidget>(GetWorld(), GameInstance->AssassinateWidgetClass))
			{
				AssassinateWidgetComponent->SetWidget(AssassinateWidget);
				AssassinateWidgetComponent->SetVisibility(false);
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

void AARPG_AICharacter::OnFinishAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AttributeComponent->IsDeath())
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		if (AARPG_Character* Character = Cast<AARPG_Character>(OtherActor))
		{
			Character->SetFinishAttackTarget(this);
		}
	}
}

void AARPG_AICharacter::OnFinishAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AttributeComponent->IsDeath())
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		if (AARPG_Character* Character = Cast<AARPG_Character>(OtherActor))
		{
			Character->SetFinishAttackTarget(nullptr);
		}
	}
}

void AARPG_AICharacter::OnAssassinateOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (AttributeComponent->IsDeath())
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		if (AARPG_Character* Character = Cast<AARPG_Character>(OtherActor))
		{
			Character->SetAssassinateTarget(this);
		}
	}
}

void AARPG_AICharacter::OnAssassinateOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AttributeComponent->IsDeath())
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		if (AARPG_Character* Character = Cast<AARPG_Character>(OtherActor))
		{
			Character->SetAssassinateTarget(nullptr);
		}
	}
}

