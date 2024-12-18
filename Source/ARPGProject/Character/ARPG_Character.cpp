
#include "ARPG_Character.h"

#include "ARPG_AICharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "ARPGProject/ARPGProject.h"

#include "ARPGProject/ARPG_GameInstance.h"
#include "ARPGProject/ARPG_GameMode.h"
#include "Component/ARPG_LockOnSystemComponent.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AARPG_Character::AARPG_Character()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// UE5 기준으로 기본 세팅
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->SetFixedBrakingDistance(200.f);
	//

	MeleeCombatComponent = CreateDefaultSubobject<UARPG_MeleeCombatComponent>(TEXT("MeleeCombatComponent"));
	MeleeCombatComponent->OnMontageEndDelegate.AddUObject(this, &AARPG_Character::OnMontageEndCallBack);
	AttributeComponent = CreateDefaultSubobject<UARPG_AttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->OnDeath.AddUObject(this, &AARPG_Character::OnDeath);

	LockOnSystemComponent = CreateDefaultSubobject<UARPG_LockOnSystemComponent>(TEXT("LockOnSystemComponent"));

	if(const ConstructorHelpers::FObjectFinder<UAnimMontage> ParkourMontage(TEXT("/Script/Engine.AnimMontage'/Game/ARPG/Characters/Barbarian/Animations/Parkour/ARPG_Parkour_Vaulting_Montage.ARPG_Parkour_Vaulting_Montage'")); ParkourMontage.Succeeded())
	{
		ParkourAnim = ParkourMontage.Object;
	}

	PrimaryActorTick.bCanEverTick = true;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}


void AARPG_Character::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AARPG_GameMode>(GetWorld()->GetAuthGameMode());
	GameInstance = Cast<UARPG_GameInstance>(GetGameInstance());
	if (IsPlayerControlled() == false)
	{
		SetCharacterKey(CharacterKey);

		AIController = Cast<AARPG_AIController>(GetController());
		if (AIController)
		{
			AIController->RunAI(CharacterData.BehaviorTree);
		}
	}
	else
	{
		SetCharacterKey(CharacterKey);
	}
	PlayerState = GetController()->GetPlayerState<AARPG_PlayerState>();
}

void AARPG_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();	

	AnimInstance = Cast<UARPG_AnimInstance>(GetMesh()->GetAnimInstance());
	
}

float AARPG_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                  AActor* DamageCauser)
{
	const float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FARPG_DamageEvent::ClassID))
	{
		const FARPG_DamageEvent& PointDamageEvent = static_cast<const FARPG_DamageEvent&>(DamageEvent);

		if (MeleeCombatComponent)
		{
			if (MeleeCombatComponent->IsGuard())
			{
				if(IsPlayerControlled())
				{
					LockOnSystemComponent->SetTarget(EventInstigator->GetPawn());
				}

				const FVector ParticleLocation = PointDamageEvent.HitInfo.Location + (GetActorForwardVector() * 50.f);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GameInstance->GuardParticleSystem, ParticleLocation,
					FRotator::ZeroRotator, FVector(1.f), true);

				MeleeCombatComponent->PlayMontage(MontageData.GuardReactionMontage);

				if (IsPlayerControlled() && MeleeCombatComponent->IsParry())
				{
					if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(EventInstigator->GetPawn()))
					{
						Interface->ParryingReaction();
						if (GameMode)
						{
							GameMode->StartSlowMotion(0.2f, 0.5f);
						}
					}
				}
				return 0;
			}
		}

		const FRotator Rotator = UKismetMathLibrary::MakeRotFromXY(PointDamageEvent.HitInfo.Normal, PointDamageEvent.HitInfo.Normal);

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), GameInstance->HitParticleSystem, PointDamageEvent.HitInfo.Location,
			Rotator, FVector(0.4f), true
		);

		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PointDamageEvent.HitInfo.Location, GetActorLocation());
		if (AnimInstance != nullptr)
		{
			AnimInstance->HitTrigger(LookAtRotation.Yaw);
		}
		MeleeCombatComponent->StopMontage();
	}

	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(EventInstigator->GetPawn()->GetActorRotation());
	const FVector LaunchVelocity = ForwardVector * 700.f;
	LaunchCharacter(LaunchVelocity, false, false);
	AttributeComponent->TakeDamage(ResultDamage);
	return ResultDamage;
}

void AARPG_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AARPG_Character::SetFinishAttackTarget(AARPG_AICharacter* InFinishAttackTarget)
{
	FinishAttackTargetActor = InFinishAttackTarget;
}

void AARPG_Character::SetAssassinateTarget(AARPG_AICharacter* InAssassinateTarget)
{
	if(AssassinateTarget)
	{
		AssassinateTarget->SetAssassinateWidget(false);
	}
	AssassinateTarget = InAssassinateTarget;
	if (AssassinateTarget)
	{
		AssassinateTarget->SetAssassinateWidget(true);
	}
}

void AARPG_Character::OnMontageEndCallBack(bool bInterrupted)
{
	bIsKnockBack = false;
	if (bIsFinishAttack)
	{
		AttributeComponent->TakeDamage(AttributeComponent->Health);
	}
}

void AARPG_Character::SetCharacterKey(const FName InCharacterKey)
{
	CharacterKey = InCharacterKey;
	GameInstance->TryGetMontageData(CharacterKey.ToString(), MontageData);
	if(GameInstance->TryGetCombatData(CharacterKey.ToString(), CombatData))
	{
		if(MeleeCombatComponent != nullptr)
		{
			MeleeCombatComponent->SetStartComboMontage(CombatData.AttackMontage);
		}
	}
	if(IsPlayerControlled())
	{
		if (FARPG_WeaponData WeaponData; GameInstance->TryGetWeaponData("Sword", WeaponData))
		{
			MainWeapon = CreateWeapon(WeaponData.WeaponClass);
			MainWeapon->SetActorHiddenInGame(false);
			WeaponAttach("Sword_Attach");
		}
	}
	else
	{
		if (GameInstance->TryGetCharacterData(CharacterKey.ToString(), CharacterData))
		{
			if (CharacterData.WeaponClass)
			{
				MainWeapon = CreateWeapon(CharacterData.WeaponClass);
				MainWeapon->SetActorHiddenInGame(false);
				WeaponAttach("Sword_Attach");
			}
		}
	}	
}

bool AARPG_Character::IsMainWeaponGrip() const
{
	return bIsMainWeaponGrip;
}

bool AARPG_Character::IsRolling() const
{
	return bRolling;
}

bool AARPG_Character::IsGuard() const
{
	if(MeleeCombatComponent)
	{
		return  MeleeCombatComponent->IsGuard();
	}
	return false;	
}

bool AARPG_Character::IsKnockBack() const
{
	return bIsKnockBack;
}

bool AARPG_Character::IsLockOnTarget() const
{
	if(IsPlayerControlled())
	{
		return LockOnSystemComponent->IsLockOnTarget();
	}
	return false;
}

AARPG_WeaponBase* AARPG_Character::CreateWeapon(const TSubclassOf<AARPG_WeaponBase>& InWeaponBase)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (AARPG_WeaponBase* SpawnedActor = GetWorld()->SpawnActor<AARPG_WeaponBase>(InWeaponBase, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
	{
		return SpawnedActor;
	}
	return nullptr;
}

void AARPG_Character::SetNextCombo(const UAnimMontage* NewNextComboMontage)
{
	MeleeCombatComponent->SetNextCombo(NewNextComboMontage);
}

void AARPG_Character::AttackCheckBegin()
{
	if(MainWeapon != nullptr)
	{
		MainWeapon->AttackCheckBegin();
	}
}

void AARPG_Character::AttackCheckEnd()
{
	if (MainWeapon != nullptr)
	{
		MainWeapon->AttackCheckEnd();
	}
}

void AARPG_Character::WeaponAttach(const FName AttachSocketName)
{
	if(MainWeapon)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		MainWeapon->AttachToComponent(GetMesh(), AttachmentRules, AttachSocketName);
	}
}

void AARPG_Character::ParryingReaction()
{
	bIsKnockBack = true;
	MeleeCombatComponent->PlayMontage(MontageData.ParryingReactionMontage);
}

void AARPG_Character::FinishAttackReaction()
{
	bIsFinishAttack = true;
	
	LockOnSystemComponent->SetTarget(nullptr);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Death"));
	if (AIController)
	{
		AIController->StopAI();
	}
	MeleeCombatComponent->PlayMontage(MontageData.FinishAttackReactionMontage);
}

void AARPG_Character::FinishAttackDeath()
{
	AttributeComponent->TakeDamage(AttributeComponent->Health);
	bIsFinishAttack = false;
}

void AARPG_Character::AssassinateReaction()
{
	bIsFinishAttack = true;
	SetAssassinateTarget(nullptr);
	LockOnSystemComponent->SetTarget(nullptr);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Death"));
	if (AIController)
	{
		AIController->StopAI();
	}
	MeleeCombatComponent->PlayMontage(MontageData.AssassinateReactionMontage);
}

void AARPG_Character::SetMotionWarping(const AActor* InTarget)
{
	FVector Direction = (InTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	float Distance = FVector::Distance(InTarget->GetActorLocation(), GetActorLocation());
	FVector TargetLocation = GetActorLocation() + Direction * (Distance - 50.f);
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InTarget->GetActorLocation());
	SetMotionWarping(TargetLocation, FRotator(0, TargetRotation.Yaw, 0));
}

void AARPG_Character::SetMotionWarping(const FVector& TargetLocation, const FRotator& TargetRotation)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("Target", TargetLocation, TargetRotation);
}

void AARPG_Character::OnDeath()
{
	if(IsPlayerControlled() == false)
	{
		if(bIsFinishAttack == false)
		{
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetMesh()->SetAllBodiesSimulatePhysics(true);
		}
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Death"));
		LockOnSystemComponent->SetTarget(nullptr);
		if(AIController)
		{
			AIController->StopAI();
		}
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GameInstance->DeathSound, GetActorLocation(), GetActorRotation());
		GameMode->IncreaseKillCount();
	}
	else
	{
		GameMode->PlayerDeath();
	}
}

void AARPG_Character::FinishAttack()
{
	FVector TargetLocation = FinishAttackTargetActor->GetActorLocation() + FinishAttackTargetActor->GetActorForwardVector() * 100.f;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, FinishAttackTargetActor->GetActorLocation());
	SetMotionWarping(TargetLocation, FRotator(0, TargetRotation.Yaw, 0));

	MeleeCombatComponent->PlayMontage(MontageData.FinishAttackMontage);
}

void AARPG_Character::Assassinate()
{
	FVector TargetLocation = AssassinateTarget->GetActorLocation() - AssassinateTarget->GetActorForwardVector() * 50.f;
	FVector OffsetLocation = TargetLocation + AssassinateTarget->GetActorRightVector() * 30.f;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, AssassinateTarget->GetActorLocation());
	SetMotionWarping(OffsetLocation, FRotator(0, TargetRotation.Yaw, 0));

	MeleeCombatComponent->PlayMontage(MontageData.AssassinateMontage);
}
