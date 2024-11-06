
#include "ARPG_Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
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

	MeleeCombatComponent = CreateDefaultSubobject<UARPG_MeleeCombatComponent>(TEXT("MeleeCombatComp"));
	MeleeCombatComponent->OnAttackEndDelegate.AddLambda([this]() -> void
	{
		bIsKnockBack = false;
		FinishAttackCollider->SetGenerateOverlapEvents(false);
		if (bIsFinishAttack)
		{
			OnDeath();
		}
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

	PrimaryActorTick.bCanEverTick = true;


	// 전방에 위치할 FinishAttackCollider 초기화
	FinishAttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("FinishAttackCollider"));
	FinishAttackCollider->SetupAttachment(RootComponent);

	// 콜라이더 크기와 위치 설정
	FinishAttackCollider->SetSphereRadius(100.0f);  // 필요에 따라 범위를 조정
	FinishAttackCollider->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	FinishAttackCollider->SetGenerateOverlapEvents(false);
	FinishAttackCollider->SetHiddenInGame(false);

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
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("ff %f"), PlayerState->GetCurrentHeath()));


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
			if (MeleeCombatComponent->IsParry())
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), ParryParticleSystem, PointDamageEvent.HitInfo.Location,
					FRotator::ZeroRotator, FVector(1.0f), true);

				if (EventInstigator->GetPawn()->Implements<UARPG_CharacterInterface>())
				{
					Execute_HitKnockBack(EventInstigator->GetPawn(), HitReactionMontageData);
					AARPG_GameMode* GameMode = Cast<AARPG_GameMode>(GetWorld()->GetAuthGameMode());
					if (GameMode)
					{
						GameMode->StartSlowMotion(0.2f, 0.5f);
					}
				}
				return 0;
			}

			if (MeleeCombatComponent->IsDefense())
			{
				MeleeCombatComponent->PlayMontage(MontageData.DefenseReactionMontage);
				return 0;
			}
		}

		const FRotator Rotator = UKismetMathLibrary::MakeRotFromXY(PointDamageEvent.HitInfo.Normal, PointDamageEvent.HitInfo.Normal);

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), HitParticleSystem, PointDamageEvent.HitInfo.Location,
			Rotator, FVector(0.4f), true
		);
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PointDamageEvent.HitInfo.Location, GetActorLocation());
		AnimInstance->HitTrigger(LookAtRotation.Yaw);
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

	/*if(TargetActor)
	{
		const FRotator CurrentRot = GetControlRotation();
		const FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator ResultRot = UKismetMathLibrary::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, 20.f);

		GetController()->SetControlRotation(ResultRot);
	}*/
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
		if(GameInstance->TryGetMontageData(CharacterKey.ToString(), MontageData))
		{
			//MeleeCombatComponent->SetMontageData(MontageData);
		}
	}
}

void AARPG_Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement 
		//AddMovementInput(ForwardDirection, MovementVector.Y);
		//AddMovementInput(RightDirection, MovementVector.X);

		// 입력 벡터에 따라 캐릭터가 이동할 방향 벡터 계산
		const FVector Direction = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);

		// 캐릭터의 이동을 처리
		if (!Direction.IsNearlyZero()) // 방향 벡터가 거의 0이 아니면
		{
			AddMovementInput(Direction);

			// 이동하는 방향으로 캐릭터가 회전하도록 처리
			DirectionRotator = Direction.Rotation();  // 이동 방향에 대한 회전값 계산
			DirectionRotator.Pitch = 0.0f;  // Pitch를 0으로 설정 (캐릭터가 위/아래로 회전하지 않게)
			DirectionRotator.Roll = 0.0f;   // Roll을 0으로 설정 (좌/우 기울기를 방지)
		}
	}
}

void AARPG_Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AARPG_Character::InputAttack()
{
	if(bEquipping)
	{
		return;
	}

	if(bCanFinishAttack)
	{
		bCanFinishAttack = false;
		if(FinishAttackTargetActor)
		{
			FVector DirectionToTarget = FinishAttackTargetActor->GetActorLocation() - GetActorLocation();
			DirectionToTarget.Z = 0; // 캐릭터의 높이는 유지하며 평면 회전만 수행

			FRotator TargetRotation = DirectionToTarget.Rotation();
			SetActorRotation(TargetRotation);
			MeleeCombatComponent->PlayMontage(MontageData.FinishAttackMontage);
			if(FinishAttackTargetActor->Implements<UARPG_CharacterInterface>())
			{
				IARPG_CharacterInterface::Execute_FinishAttack(FinishAttackTargetActor);
			}
		}
		return;
	}

	if (LockOnSystemComponent->IsLockOnTarget() == false)
	{
		SetActorRotation(DirectionRotator);
	}
	MeleeCombatComponent->InputAttack();
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
		UKismetSystemLibrary::PrintString(GetWorld(), "Input Defense !!");
	}
	else
	{
		MeleeCombatComponent->DefenseComplete();
	}
	//LaunchCharacter(GetActorForwardVector() * 1000.f, false, false);
}

void AARPG_Character::InputTargetLockOn(const FInputActionValue& Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Lcok On!!");
	if(LockOnSystemComponent == nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), *GetActorNameOrLabel());
		return;
	}
	LockOnSystemComponent->InputTargetLockOn();
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
	FString dd = AttachSocketName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Weapon Attach : %s"), *AttachSocketName.ToString());
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, AttachSocketName);
	CurrentWeapon->SetActorHiddenInGame(false);
}

void AARPG_Character::HitKnockBack_Implementation(const UAnimMontage* HitReactionMontage)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Hit Knock Back"));

	bIsKnockBack = true;
	FinishAttackCollider->SetGenerateOverlapEvents(true);
	MeleeCombatComponent->PlayMontage(MontageData.ParryingReactionMontage);
}

void AARPG_Character::FinishAttack_Implementation()
{
	bIsFinishAttack = true;
	MeleeCombatComponent->PlayMontage(MontageData.FinishAttackReactionMontage);
}

void AARPG_Character::FinishAttackDeath()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetHiddenInGame(true);
	}
	AIController->StopAI();
}

void AARPG_Character::OnDeath()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Death"));

	if(IsPlayerControlled() == false)
	{
		
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
		if(HealthWidgetComponent)
		{
			HealthWidgetComponent->SetHiddenInGame(true);
		}
		AIController->StopAI();
	}
}

void AARPG_Character::OnFinishAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( OtherActor && OtherActor != this)
	{
		AARPG_Character* Character = Cast<AARPG_Character>(OtherActor);
		if(Character)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), "OnFinishAttackOverlapBegin");

			Character->SetCanFinishAttack(true, this);
		}
	}
}

void AARPG_Character::OnFinishAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		AARPG_Character* Character = Cast<AARPG_Character>(OtherActor);
		if (Character)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), "OnFinishAttackOverlapEnd");
			Character->SetCanFinishAttack(false, nullptr);
		}
	}
}


void AARPG_Character::WeaponEquip_Implementation(const bool InEquipping)
{
	bEquipping = InEquipping;
	
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Change bEquipping: %s"), bEquipping ? TEXT("true") : TEXT("false")));
}
