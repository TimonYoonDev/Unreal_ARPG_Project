// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

AARPG_AICharacter::AARPG_AICharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName("NPC");
}

void AARPG_AICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bIsAttacking)
	{
		AttackTrace();
	}
}

void AARPG_AICharacter::AttackCheckBegin_Implementation()
{
	bIsAttacking = true;
}

void AARPG_AICharacter::AttackCheckEnd_Implementation()
{
	TargetSet.Empty();
	bIsAttacking = false;
}

void AARPG_AICharacter::AttackTrace()
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


