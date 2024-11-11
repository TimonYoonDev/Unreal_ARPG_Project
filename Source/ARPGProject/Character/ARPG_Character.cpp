
#include "ARPG_Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "KismetTraceUtils.h"
#include "ARPGProject/ARPG_GameInstance.h"
#include "ARPGProject/ARPG_GameMode.h"
#include "Component/ARPG_LockOnSystemComponent.h"
#include "Components/SphereComponent.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UARPG_CameraComponent>(TEXT("CameraComponent"));
	CameraComponent->Init(CameraBoom, FollowCamera);

	MeleeCombatComponent = CreateDefaultSubobject<UARPG_MeleeCombatComponent>(TEXT("MeleeCombatComp"));
	MeleeCombatComponent->OnAttackEndDelegate.AddLambda([this]() -> void
	{
		bIsKnockBack = false;
		FinishAttackCollider->SetGenerateOverlapEvents(false);
		if (bIsFinishAttack)
		{
			OnDeath();
		}
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		SetActorEnableCollision(true);
	});
	AttributeComponent = CreateDefaultSubobject<UARPG_AttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->OnDeath.AddUObject(this, &AARPG_Character::OnDeath);

	LockOnSystemComponent = CreateDefaultSubobject<UARPG_LockOnSystemComponent>(TEXT("LockOnSystemComponent"));
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	if (HitParticle.Succeeded())
	{
		HitParticleSystem = HitParticle.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParryParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Sparks/P_Sparks_E.P_Sparks_E'"));
	if (ParryParticle.Succeeded())
	{
		ParryParticleSystem = ParryParticle.Object;
	}
	if(const ConstructorHelpers::FObjectFinder<UAnimMontage> ParkourMontage(TEXT("/Script/Engine.AnimMontage'/Game/ARPG/Characters/Barbarian/Animations/Parkour/ARPG_Parkour_Vaulting_Montage.ARPG_Parkour_Vaulting_Montage'")); ParkourMontage.Succeeded())
	{
		ParkourAnim = ParkourMontage.Object;
	}

	if (const ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/ARPG/Audio/Combat/MSS_Death.MSS_Death'")); Sound.Succeeded())
	{
		DeathSound = Sound.Object;
	}

	if (static ConstructorHelpers::FClassFinder<AARPG_WeaponBase> Bow(TEXT("/Script/Engine.Blueprint'/Game/ARPG/Blueprints/Weapons/BP_ARPG_Weapon_Bow.BP_ARPG_Weapon_Bow_C'")); Bow.Succeeded())
	{
		BowWeaponClass = Bow.Class;
	}

	if (static ConstructorHelpers::FClassFinder<AActor> Arrow(TEXT("/Script/Engine.Blueprint'/Game/ARPG/Blueprints/Projectile/BP_Projectile_Arrow.BP_Projectile_Arrow_C'")); Arrow.Succeeded())
	{
		ArrowClass = Arrow.Class;
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

	//CameraBoom->SocketOffset = FVector(0, 0, 130);

	ArrowPos = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowPos"));
	ArrowPos->SetupAttachment(GetMesh(), "arrow_socket");
}


void AARPG_Character::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UARPG_GameInstance>(GetGameInstance());
	if (IsPlayerControlled() == false)
	{
		SetCharacterKey(FName("Quinn"));

		AIController = Cast<AARPG_AIController>(GetController());
		if (AIController)
		{
			AIController->RunAI();
		}
	}
	else
	{
		SetCharacterKey(FName("Barbarian"));
	}
	PlayerState = GetController()->GetPlayerState<AARPG_PlayerState>();
	if (PlayerState)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("AARPG_Character::BeginPlay "));
	}

	BowWeapon = CreateWeapon(BowWeaponClass);
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	BowWeapon->AttachToComponent(GetMesh(), AttachmentRules, "weapon_l_equip_socket");

	if (GameInstance)
	{
		const FString RowName("1");
		// TryGetWeaponData 호출
		if (FARPG_WeaponData WeaponData; GameInstance->TryGetWeaponData(RowName, WeaponData))
		{
			// 성공적으로 무기 데이터를 얻은 경우
			EquipWeaponArray.Add(CreateWeapon(WeaponData.WeaponClass));
			if (FARPG_CombatData CombatData; GameInstance->TryGetCombatData(WeaponData.CombatDataRowName, CombatData))
			{
				CombatDataArray.Add(CombatData);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Combat data for row %s not found"), *WeaponData.CombatDataRowName);
				CombatDataArray.Add(FARPG_CombatData());
			}
		}
		else
		{
			// 무기 데이터를 찾지 못한 경우
			UE_LOG(LogTemp, Warning, TEXT("Weapon data for row %s not found"), *RowName);
		}
		const FString RowName2("2");
		// TryGetWeaponData 호출
		if (FARPG_WeaponData WeaponData; GameInstance->TryGetWeaponData(RowName2, WeaponData))
		{
			// 성공적으로 무기 데이터를 얻은 경우
			EquipWeaponArray.Add(CreateWeapon(WeaponData.WeaponClass));
			if (FARPG_CombatData CombatData; GameInstance->TryGetCombatData(WeaponData.CombatDataRowName, CombatData))
			{
				CombatDataArray.Add(CombatData);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Combat data for row %s not found"), *WeaponData.CombatDataRowName);
				CombatDataArray.Add(FARPG_CombatData());
			}
		}
		else
		{
			// 무기 데이터를 찾지 못한 경우
			UE_LOG(LogTemp, Warning, TEXT("Weapon data for row %s not found"), *RowName);
		}
	}
	SetWeapon(0);
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
			UKismetSystemLibrary::PrintString(GetWorld(), *PointDamageEvent.HitInfo.Location.ToString());
			if (MeleeCombatComponent->IsDefense())
			{
				FVector DirectionToTarget = EventInstigator->GetPawn()->GetActorLocation() - GetActorLocation();
				DirectionToTarget.Z = 0;

				const FRotator TargetRotation = DirectionToTarget.Rotation();
				SetActorRotation(TargetRotation);

				const FVector ParticleLocation = PointDamageEvent.HitInfo.Location + (GetActorForwardVector() * 50.f);
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), ParryParticleSystem, ParticleLocation,
					FRotator::ZeroRotator, FVector(0.5f), true);

				MeleeCombatComponent->PlayMontage(MontageData.DefenseReactionMontage);

				if (MeleeCombatComponent->IsParry())
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
			GetWorld(), HitParticleSystem, PointDamageEvent.HitInfo.Location,
			Rotator, FVector(0.4f), true
		);
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PointDamageEvent.HitInfo.Location, GetActorLocation());
		if (AnimInstance != nullptr)
		{
			AnimInstance->HitTrigger(LookAtRotation.Yaw);
		}
		
	}

	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(EventInstigator->GetPawn()->GetActorRotation());
	const FVector LaunchVelocity = ForwardVector * 350.f;
	LaunchCharacter(LaunchVelocity, false, false);
	AttributeComponent->TakeDamage(ResultDamage);
	return ResultDamage;
}

void AARPG_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//ParkourScanner();
}

void AARPG_Character::ParkourScanner()
{
	for (size_t i = 0; i < 3; ++i)
	{
		FHitResult HitResult;
		FVector Start = GetActorLocation() + FVector(0, 0, i * 30);
		FVector End = GetActorForwardVector() * 180.0f + Start;
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(5.f);

		GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CollisionShape);
#if ENABLE_DRAW_DEBUG
		DrawDebugSweptSphere(GetWorld(), Start, End, 5.f, FColor::Red, false, 1.f);
#endif

		if(HitResult.IsValidBlockingHit())
		{
			ParkourScannerSub(HitResult);
			break;
		}
	}
	
}

void AARPG_Character::ParkourScannerSub(FHitResult HitResult)
{
	for (size_t i = 0; i < 5; ++i)
	{
		FVector A = HitResult.Location + FVector(0.f, 0.f, 100.f);
		FVector B = GetActorForwardVector() * (i * 50.f);
		UE_LOG(LogTemp, Warning, TEXT("test : %s / %s"), *A.ToString(), *B.ToString());
		FVector Start = A + B;
		FVector End = Start - FVector(0, 0, 100.f);
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(5.f);

		GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CollisionShape);
#if ENABLE_DRAW_DEBUG
		DrawDebugSweptSphere(GetWorld(), Start, End, 5.f, FColor::Red, false, 1.f);
#endif

		if(HitResult.IsValidBlockingHit())
		{

#if ENABLE_DRAW_DEBUG
			DrawDebugSphere(GetWorld(), HitResult.Location, 5.f, 0, FColor::Green, false, 1.f);
#endif
			float Z = HitResult.Location.Z - GetActorLocation().Z;
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.Location.Z));
			if (AARPG_GameMode* GameMode = Cast<AARPG_GameMode>(GetWorld()->GetAuthGameMode()))
			{
				GameMode->StartSlowMotion(0.2f, 0.5f);
			}
			break;
		}
	}
}


void AARPG_Character::SetCanFinishAttack(bool InCanFinishAttack, AActor* InFinishAttackTarget)
{
	bCanFinishAttack = InCanFinishAttack;
	FinishAttackTargetActor = InFinishAttackTarget;
}

void AARPG_Character::SetCharacterKey(const FName InCharacterKey)
{
	CharacterKey = InCharacterKey;
	if(MeleeCombatComponent)
	{
		GameInstance->TryGetMontageData(CharacterKey.ToString(), MontageData);
	}
}

void AARPG_Character::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (const FVector Direction = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X); Direction.IsNearlyZero() == false)
		{
			AddMovementInput(Direction);

			DirectionRotator = Direction.Rotation();
			DirectionRotator.Pitch = 0.0f;
			DirectionRotator.Roll = 0.0f;
		}
		else
		{
			DirectionRotator = FRotator::ZeroRotator;
		}
	}
}

void AARPG_Character::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AARPG_Character::InputAttack(const FInputActionValue& Value)
{
	if(Value.Get<bool>() == true)
	{
		if (bIsBowMode)
		{
			bIsBowDrawing = true;
			return;
		}

		if (bEquipping)
		{
			return;
		}

		if (bCanFinishAttack)
		{
			bCanFinishAttack = false;
			if (FinishAttackTargetActor)
			{
				FVector DirectionToTarget = FinishAttackTargetActor->GetActorLocation() - GetActorLocation();
				DirectionToTarget.Z = 0;

				const FRotator TargetRotation = DirectionToTarget.Rotation();
				SetActorRotation(TargetRotation);
				MeleeCombatComponent->PlayMontage(MontageData.FinishAttackMontage);
				if (IARPG_CharacterInterface* Interface = Cast<IARPG_CharacterInterface>(FinishAttackTargetActor))
				{
					Interface->FinishAttack();
				}
			}
			return;
		}
		LockOnSystemComponent->SetTarget(LockOnSystemComponent->FindClosestTarget());

		if (DirectionRotator.IsNearlyZero() == false)
		{
			SetActorRotation(DirectionRotator);
		}
		MeleeCombatComponent->InputAttack();
	}
	else
	{
		if(bIsBowDrawing)
		{
			// 사격!
			bIsBowDrawing = false;

			// 스폰 파라미터 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;  // 스폰한 액터의 소유자 설정
			SpawnParams.Instigator = GetInstigator();  // 인스티게이터 설정 (공격자나 원인을 의미)
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;  // 충돌 처리 방식 설정
			FVector Location = ArrowPos->GetComponentLocation() + GetActorForwardVector() * 50.f;
			AARPG_WeaponBase* Arrow = GetWorld()->SpawnActor<AARPG_WeaponBase>(ArrowClass, Location, ArrowPos->GetComponentRotation(), SpawnParams);
			Arrow->AttackCheckBegin();

			
		}
	}
	
}

void AARPG_Character::HeavyAttackStart()
{
	if (bEquipping)
	{
		return;
	}
	if (LockOnSystemComponent->IsLockOnTarget() == false)
	{
		SetActorRotation(DirectionRotator);
	}
	MeleeCombatComponent->HeavyAttack(CombatDataArray[CurrentWeaponIndex].HeavyAttackMontage);
}

void AARPG_Character::HeavyAttackCompleted()
{
	if (bEquipping)
	{
		return;
	}
	if(CombatDataArray[CurrentWeaponIndex].bChargedAttack)
	{
		MeleeCombatComponent->HeavyAttackComplete(CombatDataArray[CurrentWeaponIndex].HeavyAttackMontage);
	}
}

void AARPG_Character::InputWeaponChange(const FInputActionValue& Value)
{
	SetWeapon(CurrentWeaponIndex + 1);
}

void AARPG_Character::InputRoll(const FInputActionValue& Value)
{
	bRolling = Value.Get<bool>();
	if(bRolling)
	{
		SetActorRotation(DirectionRotator);
		PlayAnimMontage(CombatDataArray[CurrentWeaponIndex].RollMontage);
		LaunchCharacter(GetActorForwardVector() * 1000.f, false, false);
	}
}

void AARPG_Character::InputDefense(const FInputActionValue& Value)
{
	if(Value.Get<bool>())
	{
		MeleeCombatComponent->Defense();
	}
	else
	{
		MeleeCombatComponent->DefenseComplete();
	}
}

void AARPG_Character::InputTargetLockOn(const FInputActionValue& Value)
{
	if(LockOnSystemComponent == nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), *GetActorNameOrLabel());
		return;
	}
	LockOnSystemComponent->InputTargetLockOn();
}

void AARPG_Character::InputParkour(const FInputActionValue& Value)
{
	if (Value.Get<bool>() == false)
	{
		MeleeCombatComponent->PlayMontage(ParkourAnim);
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		SetActorEnableCollision(false);
		ParkourScanner();
	}
}

void AARPG_Character::InputBowMode(const FInputActionValue& Value)
{
	bIsBowMode = Value.Get<bool>();
	if (CurrentWeapon != nullptr)
	{
		
		CurrentWeapon->SetActorHiddenInGame(bIsBowMode);
	}
	if(BowWeapon != nullptr)
	{
		BowWeapon->SetActorHiddenInGame(bIsBowMode == false);
	}

	if(bIsBowMode)
	{
		CameraComponent->AimCameraMove();
	}
	else
	{
		CameraComponent->OriginCameraMove();
	}
	/*CameraBoom->TargetArmLength = bIsBowMode ? 400.0f : 250.f;
	CameraBoom->bEnableCameraLag = bIsBowMode == false;
	CameraBoom->SocketOffset = bIsBowMode ? FVector(100, 100	, 100) : FVector(0, 0, 130);
	FollowCamera->FieldOfView = bIsBowMode ? 45 : 90;*/
	GetCharacterMovement()->bUseControllerDesiredRotation = bIsBowMode;
	GetCharacterMovement()->bOrientRotationToMovement = bIsBowMode == false;
	GetCharacterMovement()->MaxWalkSpeed = bIsBowMode ? 250 : 500;
	
}

void AARPG_Character::SetWeapon(int NextWeaponIndex)
{
	
	if(AnimInstance == nullptr)
	{
		return;
	}
	if(MeleeCombatComponent->GetMontagePlaying())
	{
		return;
	}
	if(CombatDataArray.Num() == 0)
	{
		return;
	}

	AnimInstance->EquipWeaponTrigger();
	NextWeaponIndex = NextWeaponIndex < EquipWeaponArray.Num() ? NextWeaponIndex : 0;
	CurrentWeaponIndex = NextWeaponIndex;
	FARPG_CombatData CombatData = CombatDataArray[CurrentWeaponIndex];
	const int CombatPoseIndex = CombatData.CombatPoseIndex;
	MeleeCombatComponent->SetStartComboMontage(CombatData.AttackMontage);

	AnimInstance->SetEquipWeaponIndex(CombatPoseIndex);

	if(CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}	
	CurrentWeapon = EquipWeaponArray[CurrentWeaponIndex];

	bEquipping = true;
}

bool AARPG_Character::IsRolling() const
{
	return bRolling;
}

bool AARPG_Character::IsDefending() const
{
	if(MeleeCombatComponent)
	{
		return  MeleeCombatComponent->IsDefense();
	}
	return false;	
}

bool AARPG_Character::IsKnockBack() const
{
	return bIsKnockBack;
}

AARPG_WeaponBase* AARPG_Character::CreateWeapon(TSubclassOf<AARPG_WeaponBase> InWeaponBase)
{
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// 스폰 파라미터 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;  // 스폰한 액터의 소유자 설정
			SpawnParams.Instigator = GetInstigator();  // 인스티게이터 설정 (공격자나 원인을 의미)
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;  // 충돌 처리 방식 설정

			// 액터 스폰
			if (AARPG_WeaponBase* SpawnedActor = World->SpawnActor<AARPG_WeaponBase>(InWeaponBase, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
			{
				SpawnedActor->SetActorHiddenInGame(true);
				return SpawnedActor;
			}
			
		}
		return nullptr;
	}
}

void AARPG_Character::SetNextCombo_Implementation(const UAnimMontage* NewNextComboMontage)
{
	MeleeCombatComponent->SetNextCombo(NewNextComboMontage);
}

void AARPG_Character::AttackCheckBegin_Implementation()
{
	if(CurrentWeapon != nullptr)
	{
		CurrentWeapon->AttackCheckBegin();
	}
}

void AARPG_Character::AttackCheckEnd_Implementation()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->AttackCheckEnd();
	}
}

void AARPG_Character::WeaponAttach_Implementation(const FName AttachSocketName)
{
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, AttachSocketName);
	CurrentWeapon->SetActorHiddenInGame(false);
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
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Death"));
	AttributeComponent->TakeDamage(AttributeComponent->Health);
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetHiddenInGame(true);
	}
	AIController->StopAI();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation());
}

void AARPG_Character::OnDeath()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Death"));

	if(IsPlayerControlled() == false)
	{
		
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Death"));
		if(HealthWidgetComponent)
		{
			HealthWidgetComponent->SetHiddenInGame(true);
		}
		AIController->StopAI();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation());
	}
}

void AARPG_Character::OnFinishAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( OtherActor && OtherActor != this)
	{
		if(AARPG_Character* Character = Cast<AARPG_Character>(OtherActor))
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), "OnFinishAttackOverlapBegin");
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
			//UKismetSystemLibrary::PrintString(GetWorld(), "OnFinishAttackOverlapEnd");
			Character->SetCanFinishAttack(false, nullptr);
		}
	}
}


void AARPG_Character::WeaponEquip_Implementation(const bool InEquipping)
{
	bEquipping = InEquipping;
	
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Change bEquipping: %s"), bEquipping ? TEXT("true") : TEXT("false")));
}
