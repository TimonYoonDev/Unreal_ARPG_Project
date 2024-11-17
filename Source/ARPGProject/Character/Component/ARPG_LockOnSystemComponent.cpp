#include "ARPG_LockOnSystemComponent.h"

#include "ARPGProject/Character/ARPG_Character.h"
#include "ARPGProject/Character/ARPG_EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


UARPG_LockOnSystemComponent::UARPG_LockOnSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UARPG_LockOnSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AARPG_Character>(GetOwner());
	SetMovementLockOnTarget();
	
}


void UARPG_LockOnSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (OwnerCharacter == nullptr)
	{
		return;
	}
	if (TargetCharacter)
	{
		if (TargetCharacter->GetAttributeComponent()->IsDeath())
		{
			SetTarget(FindClosestTarget());
			return;
		}
		const FRotator CurrentRot = OwnerCharacter->GetActorRotation();// GetControlRotation();
		const FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), TargetCharacter->GetActorLocation());
		const FRotator ResultRot = UKismetMathLibrary::RInterpTo(CurrentRot, TargetRot, DeltaTime, 20.f);

		OwnerCharacter->SetActorRotation(ResultRot);// GetController()->SetControlRotation(ResultRot);
	}
}

void UARPG_LockOnSystemComponent::InputTargetLockOn()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}
	if(IsLockOnTarget())
	{
		SetTarget(nullptr);
		return;
	}


	SetTarget(FindForwardClosestTarget());
}

bool UARPG_LockOnSystemComponent::IsLockOnTarget() const
{
	return TargetCharacter ? true : false;
}

AActor* UARPG_LockOnSystemComponent::FindForwardClosestTarget() const
{
	if (OwnerCharacter == nullptr)
	{
		return nullptr;
	}
	FVector StartPos = OwnerCharacter->GetActorLocation();
	FCollisionObjectQueryParams COQP;
	COQP.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionShape CS;
	CS.SetSphere(SphereRadius);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(GetOwner());

	TObjectPtr<AActor> ClosestTarget = nullptr;
	float ClosestDistance = SphereRadius;
	FVector ForwardVector = OwnerCharacter->GetController()->GetControlRotation().Vector();
	if (TArray<FHitResult> HitResults; GetWorld()->SweepMultiByObjectType(HitResults, StartPos, StartPos, FQuat::Identity, COQP, CS, CQP))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("FindForwardClosestTarget"));
		for (auto HitResult : HitResults)
		{
			FVector DirectionToEnemy = HitResult.GetActor()->GetActorLocation() - StartPos;
			const float DistanceToEnemy = DirectionToEnemy.Size();
			DirectionToEnemy.Normalize();
			const float DotProduct = FVector::DotProduct(ForwardVector, DirectionToEnemy);

			if (const float Angle = FMath::RadiansToDegrees(acos(DotProduct)); Angle > MaxLockOnAngle)
			{
				continue;
			}

			FVector EndPos = StartPos + DirectionToEnemy * ClosestDistance;

			DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, 1.0f, 0, 1.0f);

			if (FHitResult LineTraceHitResult; GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, StartPos, EndPos, ECC_Pawn, CQP))
			{
				if (HitResult.GetActor() == LineTraceHitResult.GetActor())
				{
					if (DistanceToEnemy < ClosestDistance)
					{
						ClosestTarget = HitResult.GetActor();
						ClosestDistance = DistanceToEnemy;
					}
				}
			}
		}
	}
	return ClosestTarget;
}

AActor* UARPG_LockOnSystemComponent::FindClosestTarget() const
{
	if (OwnerCharacter == nullptr)
	{
		return nullptr;
	}
	const FVector StartPos = OwnerCharacter->GetActorLocation();
	FCollisionObjectQueryParams COQP;
	COQP.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionShape CS;
	CS.SetSphere(SphereRadius);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(GetOwner());

	TObjectPtr<AActor> ClosestTarget = nullptr;
	float ClosestDistance = SphereRadius;
	if (TArray<FHitResult> HitResults; GetWorld()->SweepMultiByObjectType(HitResults, StartPos, StartPos, FQuat::Identity, COQP, CS, CQP))
	{
		for (auto HitResult : HitResults)
		{
			FVector DirectionToEnemy = HitResult.GetActor()->GetActorLocation() - StartPos;
			const float DistanceToEnemy = DirectionToEnemy.Size();
			DirectionToEnemy.Normalize();
			
			FVector EndPos = StartPos + DirectionToEnemy * ClosestDistance;

			DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, 1.0f, 0, 1.0f);

			if (FHitResult LineTraceHitResult; GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, StartPos, EndPos, ECC_Pawn, CQP))
			{
				if (HitResult.GetActor() == LineTraceHitResult.GetActor())
				{
					if (DistanceToEnemy < ClosestDistance)
					{
						ClosestTarget = HitResult.GetActor();
						ClosestDistance = DistanceToEnemy;
					}
				}
			}
		}
	}
	return ClosestTarget;
}

void UARPG_LockOnSystemComponent::SetTarget(AActor* NewTargetActor)
{
	if (TargetCharacter)
	{
		//TargetCharacter->SetLockOnWidget(false);
	}

	if (NewTargetActor != nullptr)
	{
		TargetCharacter = Cast<AARPG_Character>(NewTargetActor);
		if (TargetCharacter)
		{
			//TargetCharacter->SetLockOnWidget(true);
		}
	}
	else
	{
		TargetCharacter = nullptr;
	}

	SetMovementLockOnTarget();
}

void UARPG_LockOnSystemComponent::SetMovementLockOnTarget() const
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = IsLockOnTarget();
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !IsLockOnTarget();
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = IsLockOnTarget() ? 250 : 500;
}

