// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_Projectile.h"

#include "Character/ARPG_Character.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARPG_Projectile::AARPG_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Pivot");
	SphereComponent->SetupAttachment(RootComponent);
	

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(SphereComponent);

	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(StaticMeshComponent);
	ParticleSystem->SetTemplate(TrailParticle);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->Velocity = FVector::Zero();
}

void AARPG_Projectile::SetVelocity(FVector Velocity)
{
	ProjectileMovementComponent->Velocity = Velocity;
}

void AARPG_Projectile::AttackCheckBegin()
{
	IsAttackCheck = true;
	ParticleSystem->BeginTrails(FName("B"), FName("E"), ETrailWidthMode_FromCentre, 2.f);
}

void AARPG_Projectile::AttackCheckEnd()
{
	IsAttackCheck = false;
	ParticleSystem->EndTrails();
}

void AARPG_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARPG_Projectile::AttackTrace()
{
	FHitResult OutHit;
	const FVector BeginSocketLocation = StaticMeshComponent->GetSocketLocation(FName("B"));
	const FVector EndSocketLocation = StaticMeshComponent->GetSocketLocation(FName("E"));
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FCollisionObjectQueryParams COQP;
	COQP.AddObjectTypesToQuery(ECC_Pawn);
	COQP.AddObjectTypesToQuery(ECC_WorldStatic);
	GetWorld()->SweepSingleByObjectType(OutHit, BeginSocketLocation, EndSocketLocation, FQuat::Identity, COQP, FCollisionShape::MakeSphere(12.f), QueryParams);

	FName TargetTag = GetOwner()->ActorHasTag("Player") ? "Enemy" : "Player";

	if(OutHit.bBlockingHit)
	{
		ProjectileMovementComponent->SetActive(false);
		if (OutHit.GetActor()->ActorHasTag(TargetTag))
		{
			FPointDamageEvent DamageEventBase;
			DamageEventBase.HitInfo = OutHit;
			FString HitActorName = OutHit.GetActor()->GetName();

			AARPG_Character* Character = Cast<AARPG_Character>(OutHit.GetActor());
			FName BoneName = Character->GetMesh()->FindClosestBone(OutHit.Location);
			const FAttachmentTransformRules AttachmentRules(
				EAttachmentRule::SnapToTarget, 
				EAttachmentRule::KeepWorld, 
				EAttachmentRule::KeepWorld, true);
			this->AttachToComponent(Character->GetMesh(), AttachmentRules, BoneName);
			OutHit.GetActor()->TakeDamage(10, DamageEventBase, GetOwner()->GetInstigatorController(), this);
		}
		AttackCheckEnd();
	}

}
// Called every frame
void AARPG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsAttackCheck)
	{
		AttackTrace();
	}
}

