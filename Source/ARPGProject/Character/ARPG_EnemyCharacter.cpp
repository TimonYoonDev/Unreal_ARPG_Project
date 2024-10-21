// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_EnemyCharacter.h"

#include "ARPG_AIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AARPG_EnemyCharacter::AARPG_EnemyCharacter()
{
	AIControllerClass = AARPG_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetRelativeLocation(FVector(0, 0, 180));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
	/*if(ConstructorHelpers::FClassFinder<UUserWidget> Wb_ActorHealthBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ARPG/Blueprints/UI/WB_ActorHeathBar.WB_ActorHeathBar_C'")); Wb_ActorHealthBar.Succeeded())
	{
		HPBarWidget->SetWidgetClass(Wb_ActorHealthBar.Class);
		HPBarWidget->SetDrawSize(FVector2D(150,50));
		
	}*/
}

void AARPG_EnemyCharacter::SetWalkSpeed_Implementation(const float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}

void AARPG_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetComponent)
	{
		if (HealthBarWidgetClass)
		{
			HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
		}

		if (HealthBarWidget)
		{
			WidgetComponent->SetWidget(HealthBarWidget);
			UpdateHealthBar();
		}
	}
}

void AARPG_EnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*ConstructorHelpers::FClassFinder<UHealthBarWidget> WB_HealthBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ARPG/Blueprints/UI/WB_HealthBarWidget.WB_HealthBarWidget_C'"));
	if (HealthBarWidgetClass == nullptr)
	{
		HealthBarWidgetClass = WB_HealthBar.Class;
	}*/

	


	//if (HealthBarWidget)
	//{
	//	HealthBarWidget->AddToViewport();
	//	// 위치 조정 (예: 캐릭터의 머리 위에 표시)
	//	FVector2D ScreenPosition;
	//	APlayerController* PC = Cast<APlayerController>(GetController());
	//	if (PC)
	//	{
	//		PC->ProjectWorldLocationToScreen(GetActorLocation() + FVector(0, 0, 100), ScreenPosition);
	//		HealthBarWidget->SetPositionInViewport(ScreenPosition);
	//	}
	//}
	///*StatComponent->OnHPIsZeroDelegate.AddLambda([this]()->void
	//{
	//		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Die"));
	//		SetActorEnableCollision(false);
	//});*/
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
}
