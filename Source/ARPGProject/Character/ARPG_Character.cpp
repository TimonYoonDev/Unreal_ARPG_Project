
#include "ARPG_Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "MeleeCombatComponent.h"
#include "ARPGProject/ARPG_GameInstance.h"
#include "ARPGProject/Animation/ARPG_AnimInstance.h"
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

	MeleeCombatComponent = CreateDefaultSubobject<UMeleeCombatComponent>(TEXT("MeleeCombatComp"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	if (HitParticle.Succeeded())
	{
		HitParticleSystem = HitParticle.Object;
	}

}


void AARPG_Character::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AARPG_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (const UARPG_GameInstance* GameInstance = Cast<UARPG_GameInstance>(GetGameInstance()))
	{
		const FString RowName("1");
		// TryGetWeaponData 호출
		if (FARPG_WeaponData WeaponData; GameInstance->TryGetWeaponData(RowName, WeaponData))
		{
			// 성공적으로 무기 데이터를 얻은 경우
			EquipWeapons.Add(CreateWeapon(WeaponData.WeaponClass));
			if (FARPG_CombatData CombatData; GameInstance->TryGetCombatData(WeaponData.CombatDataRowName, CombatData))
			{
				CombatDatas.Add(CombatData);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Combat data for row %s not found"), *WeaponData.CombatDataRowName);
				CombatDatas.Add(FARPG_CombatData());
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
			EquipWeapons.Add(CreateWeapon(WeaponData.WeaponClass));
			if (FARPG_CombatData CombatData; GameInstance->TryGetCombatData(WeaponData.CombatDataRowName, CombatData))
			{
				CombatDatas.Add(CombatData);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Combat data for row %s not found"), *WeaponData.CombatDataRowName);
				CombatDatas.Add(FARPG_CombatData());
			}
		}
		else
		{
			// 무기 데이터를 찾지 못한 경우
			UE_LOG(LogTemp, Warning, TEXT("Weapon data for row %s not found"), *RowName);
		}
	}

	AnimInstance = Cast<UARPG_AnimInstance>(GetMesh()->GetAnimInstance());
	SetWeapon(0);
}

float AARPG_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                  AActor* DamageCauser)
{
	const float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 먼저 DamageEvent가 FPointDamageEvent인지 확인
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent& PointDamageEvent = static_cast<const FPointDamageEvent&>(DamageEvent);

		FRotator Rotator = UKismetMathLibrary::MakeRotFromXY(PointDamageEvent.HitInfo.Normal, PointDamageEvent.HitInfo.Normal);
				
		// 파티클 이펙트 생성
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),                   // 월드
			HitParticleSystem,            // 생성할 파티클 시스템
			PointDamageEvent.HitInfo.Location,  // 이펙트를 생성할 위치 (FVector)
			Rotator,        // 회전값 (FRotator)
			FVector(0.4f),
			
			true                          // 자동 파괴 여부 (기본값 true)
		);
		
		//const FRotator LookAtRotation = UKismetMathLibrary::FindRelativeLookAtRotation(DamageCauser->GetActorTransform(), GetActorLocation());
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PointDamageEvent.HitInfo.Location, GetActorLocation());
		AnimInstance->HitTrigger(LookAtRotation.Yaw);
		
	}
	
	//const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(DamageCauser->GetActorRotation());
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(EventInstigator->GetPawn()->GetActorRotation());
	const FVector LaunchVelocity = ForwardVector * 350.f;
	LaunchCharacter(LaunchVelocity, false, false);

	return ResultDamage;
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
		// add yaw and pitch input to controller
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
	SetActorRotation(DirectionRotator);
	MeleeCombatComponent->InputAttack();
}

void AARPG_Character::HeavyAttackHold()
{
	if (bEquipping)
	{
		return;
	}
	//UKismetSystemLibrary::PrintString(GetWorld(), "HeavyAttackHold");
	SetActorRotation(DirectionRotator);
	MeleeCombatComponent->HeavyAttack(CombatDatas[CurrentWeaponIndex].HeavyAttackMontage);
	
}

void AARPG_Character::HeavyAttackCompleted()
{
	if (bEquipping)
	{
		return;
	}
	if(CombatDatas[CurrentWeaponIndex].bChargedAttack)
	{
		MeleeCombatComponent->HeavyAttackComplete(CombatDatas[CurrentWeaponIndex].HeavyAttackMontage);
	}
	
	//UKismetSystemLibrary::PrintString(GetWorld(), "HeavyAttackCompleted");
}

void AARPG_Character::WeaponChange()
{
	
	int NextWeaponIndex = CurrentWeaponIndex + 1;
	SetWeapon(NextWeaponIndex);
	
}

void AARPG_Character::InputRoll(const FInputActionValue& Value)
{
	bRolling = Value.Get<bool>();
	UKismetSystemLibrary::PrintString(GetWorld(),  FString::Printf(TEXT("Input Roll : %ls"), bRolling ? TEXT("true") : TEXT("false")));
	if(bRolling)
	{
		SetActorRotation(DirectionRotator);
		PlayAnimMontage(CombatDatas[CurrentWeaponIndex].RollMontage);
		LaunchCharacter(GetActorForwardVector() * 1000.f, false, false);
	}
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
	if(CombatDatas.Num() == 0)
	{
		return;
	}

	AnimInstance->EquipWeaponTrigger();
	NextWeaponIndex = NextWeaponIndex < EquipWeapons.Num() ? NextWeaponIndex : 0;
	CurrentWeaponIndex = NextWeaponIndex;
	FARPG_CombatData CombatData = CombatDatas[CurrentWeaponIndex];
	const int CombatPoseIndex = CombatData.CombatPoseIndex;
	MeleeCombatComponent->SetStartComboMontage(CombatData.AttackMontage);

	AnimInstance->SetEquipWeaponIndex(CombatPoseIndex);

	if(CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}	
	CurrentWeapon = EquipWeapons[CurrentWeaponIndex];

	bEquipping = true;
}

bool AARPG_Character::IsRolling() const
{
	return bRolling;
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
				//FName SocketName("weapon_r_unequip_socket");
				//WeaponAttach_Implementation(SocketName);
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

void AARPG_Character::WeaponEquip_Implementation(const bool InEquipping)
{
	bEquipping = InEquipping;
	
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Change bEquipping: %s"), bEquipping ? TEXT("true") : TEXT("false")));
}
