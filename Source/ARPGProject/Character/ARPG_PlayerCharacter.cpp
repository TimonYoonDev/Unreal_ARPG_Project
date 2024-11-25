// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PlayerCharacter.h"

#include "InputActionValue.h"
#include "KismetAnimationLibrary.h"
#include "KismetTraceUtils.h"
#include "ARPGProject/ARPG_GameMode.h"
#include "ARPGProject/ARPG_InteractableInterface.h"
#include "Camera/CameraComponent.h"
#include "Component/ARPG_CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AARPG_PlayerCharacter::AARPG_PlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UARPG_CameraComponent>(TEXT("CameraComponent"));
	CameraComponent->Init(CameraBoom, FollowCamera);


	ArrowPos = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowPos"));
	ArrowPos->SetupAttachment(GetMesh(), "Arrow_Grip");


	if (static ConstructorHelpers::FClassFinder<AARPG_WeaponBase> Bow(TEXT("/Script/Engine.Blueprint'/Game/ARPG/Blueprints/Weapons/BP_ARPG_Weapon_Bow.BP_ARPG_Weapon_Bow_C'")); Bow.Succeeded())
	{
		BowWeaponClass = Bow.Class;
	}

	if (static ConstructorHelpers::FClassFinder<AARPG_Projectile> Arrow(TEXT("/Script/Engine.Blueprint'/Game/ARPG/Blueprints/Projectile/BP_Projectile_Arrow.BP_Projectile_Arrow_C'")); Arrow.Succeeded())
	{
		ArrowClass = Arrow.Class;
	}

	if (static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/ARPG/Audio/Combat/MSS_BowDraw.MSS_BowDraw'")); Sound.Succeeded())
	{
		BowDrawSound = Sound.Object;
	}

	if (static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/ARPG/Audio/Combat/MSS_BowShoot.MSS_BowShoot'")); Sound.Succeeded())
	{
		BowShootSound = Sound.Object;
	}
}

void AARPG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FARPG_WeaponData WeaponData; GameInstance->TryGetWeaponData("Bow", WeaponData))
	{
		BowWeapon = CreateWeapon(WeaponData.WeaponClass);
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		BowWeapon->AttachToComponent(GetMesh(), AttachmentRules, "Bow_Grip");
		BowWeapon->SetActorHiddenInGame(true);
	}
}

void AARPG_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    InteractWithObject();
	if(PressBowDrawingPower < PressBowDrawingMaxPower)
	{
		PressBowDrawingPower += DeltaSeconds;
	}
}

void AARPG_PlayerCharacter::Move(const FInputActionValue& Value)
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

void AARPG_PlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AARPG_PlayerCharacter::InputLightAttack(const FInputActionValue& Value)
{
	if(GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (Value.Get<bool>())
	{
		if (bIsBowMode)
		{
			bIsBowDrawing = true;
			BowDrawAudio = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BowDrawSound, GetActorLocation());
			BowDrawAudio->Play();
			PressBowDrawingPower = 0.f;
			return;
		}

		if (bIsMainWeaponGrip == false)
		{
			bIsMainWeaponGrip = true;
			WeaponAttach("Sword_Grip");
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

		/*if (DirectionRotator.IsNearlyZero() == false)
		{
			SetActorRotation(DirectionRotator);
		}*/
		MeleeCombatComponent->InputAttack();
	}
	else
	{
		if (bIsBowMode && bIsBowDrawing)
		{
			bIsBowDrawing = false;
			BowDrawAudio->Stop();
			ShootArrow();
		}		
	}
}

void AARPG_PlayerCharacter::InputHeavyAttack(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if(Value.Get<bool>())
	{
		if (bIsMainWeaponGrip == false)
		{
			return;
		}
		if (LockOnSystemComponent->IsLockOnTarget() == false)
		{
			SetActorRotation(DirectionRotator);
		}
		MeleeCombatComponent->HeavyAttack(CombatData.HeavyAttackMontage);
	}
	else
	{
		if (bIsMainWeaponGrip == false)
		{
			return;
		}
		if (CombatData.bChargedAttack)
		{
			MeleeCombatComponent->HeavyAttackComplete(CombatData.HeavyAttackMontage);
		}
	}
}

void AARPG_PlayerCharacter::InputRoll(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (MeleeCombatComponent->IsMontagePlaying())
	{
		return;
	}

	if (Value.Get<bool>())
	{
		if (bRolling)
		{
			return;
		}

		bRolling = true;
		LaunchCharacter(GetLastMovementInputVector() * 700.f, false, false);
		FTimerHandle OutHandle;
		GetWorld()->GetTimerManager().SetTimer(OutHandle, [this]()->void { bRolling = false; }, 0.5f, false);
	}
}

void AARPG_PlayerCharacter::InputGuard(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (Value.Get<bool>())
	{
		if (MeleeCombatComponent->IsMontagePlaying())
		{
			return;
		}
		bIsMainWeaponGrip = true;
		WeaponAttach("Sword_Grip");
		MeleeCombatComponent->Guard();
	}
	else
	{
		MeleeCombatComponent->GuardComplete();
	}
}

void AARPG_PlayerCharacter::InputTargetLockOn(const FInputActionValue& Value)
{
	LockOnSystemComponent->InputTargetLockOn();
}

void AARPG_PlayerCharacter::InputParkour(const FInputActionValue& Value)
{
	if (Value.Get<bool>() == false)
	{
		MeleeCombatComponent->PlayMontage(ParkourAnim);
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		SetActorEnableCollision(false);
		ParkourScanner();
	}
}

void AARPG_PlayerCharacter::InputBowMode(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	bIsBowMode = Value.Get<bool>();
	OnChangedBowAimMode.Broadcast(bIsBowMode);
	if (MainWeapon != nullptr)
	{
		MainWeapon->SetActorHiddenInGame(bIsBowMode);
	}
	if (BowWeapon != nullptr)
	{
		BowWeapon->SetActorHiddenInGame(bIsBowMode == false);
	}
	if(ArrowProjectile)
	{
		ArrowProjectile->SetActorHiddenInGame(bIsBowMode == false);
	}

	CreateArrowProjectile();

	if (bIsBowMode)
	{
		CameraComponent->AimCameraMove();
	}
	else
	{
		CameraComponent->OriginCameraMove();
	}
	LockOnSystemComponent->SetTarget(nullptr);
	GetCharacterMovement()->bUseControllerDesiredRotation = bIsBowMode;
	GetCharacterMovement()->bOrientRotationToMovement = bIsBowMode == false;
	GetCharacterMovement()->MaxWalkSpeed = bIsBowMode ? 250 : 500;
}

void AARPG_PlayerCharacter::CreateArrowProjectile()
{
	if (ArrowProjectile == nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (ArrowProjectile = GetWorld()->SpawnActor<AARPG_Projectile>(ArrowClass, GetTransform(), SpawnParams); ArrowProjectile)
		{
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			ArrowProjectile->AttachToComponent(GetMesh(), AttachmentRules, "Arrow_Grip");
		}
	}
}

FVector AARPG_PlayerCharacter::GetAimLocation() const
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

	if (HitResult.bBlockingHit)
	{
		return HitResult.ImpactPoint;
	}
	else
	{
		return TraceEnd;
	}
}

void AARPG_PlayerCharacter::ShootArrow()
{
	const FVector BowSocketLocation = ArrowPos->GetComponentLocation();
	const FVector AimLocation = GetAimLocation();
	const FVector AimDirection = (AimLocation - BowSocketLocation).GetSafeNormal();

	if (ArrowProjectile)
	{
		FDetachmentTransformRules DTR(EDetachmentRule::KeepWorld, false);
		ArrowProjectile->DetachFromActor(DTR);
		PressBowDrawingPower = UKismetMathLibrary::Clamp(PressBowDrawingPower, 1, PressBowDrawingMaxPower);
		ArrowProjectile->SetVelocity(AimDirection * PressBowDrawingPower * 2000.f);
		ArrowProjectile->AttackCheckBegin();
		ArrowProjectile = nullptr;
		CreateArrowProjectile();

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BowShootSound, GetActorLocation(), GetActorRotation());
	}
}

void AARPG_PlayerCharacter::ParkourScanner()
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

		if (HitResult.IsValidBlockingHit())
		{
			ParkourScannerSub(HitResult);
			break;
		}
	}
}

void AARPG_PlayerCharacter::ParkourScannerSub(FHitResult HitResult)
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

		if (HitResult.IsValidBlockingHit())
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

void AARPG_PlayerCharacter::InteractWithObject()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * 500.f;

	const FCollisionQueryParams CollisionParams;
    if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
    {
        if (IARPG_InteractableInterface* InteractableObject = Cast<IARPG_InteractableInterface>(HitResult.GetActor()))
        {
            InteractableObject->Interact(this);
        }
    }

    DrawDebugLine(GetWorld(), Start, End, FColor::Green);
}
