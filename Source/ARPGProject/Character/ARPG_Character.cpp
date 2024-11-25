
#include "ARPG_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"

#include "ARPGProject/ARPG_GameInstance.h"
#include "ARPGProject/ARPG_GameMode.h"
#include "Component/ARPG_LockOnSystemComponent.h"
#include "Components/SphereComponent.h"

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
	MeleeCombatComponent->OnMontageEndDelegate.AddLambda([this]() -> void
	{
		bIsKnockBack = false;
		FinishAttackCollider->SetGenerateOverlapEvents(false);
		/*if (bIsFinishAttack)
		{
			OnDeath();
		}*/
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		SetActorEnableCollision(true);
	});
	AttributeComponent = CreateDefaultSubobject<UARPG_AttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->OnDeath.AddUObject(this, &AARPG_Character::OnDeath);

	LockOnSystemComponent = CreateDefaultSubobject<UARPG_LockOnSystemComponent>(TEXT("LockOnSystemComponent"));

	if(const ConstructorHelpers::FObjectFinder<UAnimMontage> ParkourMontage(TEXT("/Script/Engine.AnimMontage'/Game/ARPG/Characters/Barbarian/Animations/Parkour/ARPG_Parkour_Vaulting_Montage.ARPG_Parkour_Vaulting_Montage'")); ParkourMontage.Succeeded())
	{
		ParkourAnim = ParkourMontage.Object;
	}

	PrimaryActorTick.bCanEverTick = true;


	// 전방에 위치할 FinishAttackCollider 초기화
	FinishAttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("FinishAttackCollider"));
	FinishAttackCollider->SetupAttachment(RootComponent);

	// 콜라이더 크기와 위치 설정
	FinishAttackCollider->SetSphereRadius(100.0f);  // 필요에 따라 범위를 조정
	FinishAttackCollider->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	FinishAttackCollider->SetGenerateOverlapEvents(false);

	// 오버랩 이벤트 바인딩
	FinishAttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AARPG_Character::OnFinishAttackOverlapBegin);
	FinishAttackCollider->OnComponentEndOverlap.AddDynamic(this, &AARPG_Character::OnFinishAttackOverlapEnd);

	
}


void AARPG_Character::BeginPlay()
{
	Super::BeginPlay();
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

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent& PointDamageEvent = static_cast<const FPointDamageEvent&>(DamageEvent);

		if (MeleeCombatComponent)
		{
			if (MeleeCombatComponent->IsGuard())
			{
				LockOnSystemComponent->SetTarget(EventInstigator->GetPawn());

				const FVector ParticleLocation = PointDamageEvent.HitInfo.Location + (GetActorForwardVector() * 50.f);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GameInstance->GuardParticleSystem, ParticleLocation,
					FRotator::ZeroRotator, FVector(1.f), true);

				MeleeCombatComponent->PlayMontage(MontageData.GuardReactionMontage);

				if (IsPlayerControlled() && MeleeCombatComponent->IsParry())
				{
					if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(EventInstigator->GetPawn()))
					{
						Interface->ParryingReaction();
						if (AARPG_GameMode* GameMode = Cast<AARPG_GameMode>(GetWorld()->GetAuthGameMode()))
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

void AARPG_Character::SetCanFinishAttack(bool InCanFinishAttack, AActor* InFinishAttackTarget)
{
	bCanFinishAttack = InCanFinishAttack;
	FinishAttackTargetActor = InFinishAttackTarget;
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
	FinishAttackCollider->SetGenerateOverlapEvents(true);
	MeleeCombatComponent->PlayMontage(MontageData.ParryingReactionMontage);
}

void AARPG_Character::FinishAttack()
{
	bIsFinishAttack = true;
	MeleeCombatComponent->PlayMontage(MontageData.FinishAttackReactionMontage);
}

void AARPG_Character::FinishAttackDeath()
{
	AttributeComponent->TakeDamage(AttributeComponent->Health);
	bIsFinishAttack = false;
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
		AIController->StopAI();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GameInstance->DeathSound, GetActorLocation(), GetActorRotation());
	}
}

void AARPG_Character::OnFinishAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( OtherActor && OtherActor != this)
	{
		if(AARPG_Character* Character = Cast<AARPG_Character>(OtherActor))
		{
			Character->SetCanFinishAttack(true, this);
		}
	}
}

void AARPG_Character::OnFinishAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (AARPG_Character* Character = Cast<AARPG_Character>(OtherActor))
		{
			Character->SetCanFinishAttack(false, nullptr);
		}
	}
}
