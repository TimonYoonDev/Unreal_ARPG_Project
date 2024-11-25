#include "ARPG_NoWeaponEnemyCharacter.h"

#include "Engine/DamageEvents.h"


AARPG_NoWeaponEnemyCharacter::AARPG_NoWeaponEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARPG_NoWeaponEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARPG_NoWeaponEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsAttacking)
	{
		AttackTrace();
	}
}

void AARPG_NoWeaponEnemyCharacter::AttackCheckBegin()
{
	bIsAttacking = true;
}

void AARPG_NoWeaponEnemyCharacter::AttackCheckEnd()
{
	TargetSet.Empty();
	bIsAttacking = false;
}

void AARPG_NoWeaponEnemyCharacter::AttackTrace()
{
	TArray<FHitResult> OutHits;
	const FVector BeginSocketLocation = GetMesh()->GetSocketLocation(FName("B"));
	const FVector EndSocketLocation = GetMesh()->GetSocketLocation(FName("E"));
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->SweepMultiByObjectType(OutHits, BeginSocketLocation, EndSocketLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(12.f), QueryParams);

	FName TargetTag = GetOwner()->ActorHasTag("Player") ? "Enemy" : "Player";

	for (auto OutHit : OutHits)
	{
		auto StatID = OutHit.GetActor()->GetFNameForStatID();
		if (TargetSet.Contains(StatID.ToString()))
		{
			continue;
		}
		TargetSet.Add(StatID.ToString());

		if (OutHit.GetActor()->ActorHasTag(TargetTag))
		{
			FPointDamageEvent DamageEventBase;
			DamageEventBase.HitInfo = OutHit;
			FString HitActorName = OutHit.GetActor()->GetName();

			OutHit.GetActor()->TakeDamage(10, DamageEventBase, GetOwner()->GetInstigatorController(), this);
		}
		UE_LOG(LogTemp, Warning, TEXT("Target %s"), *StatID.ToString());
	}
}



