// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_Projectile.h"

#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARPG_Projectile::AARPG_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Pivot");
	SceneComponent->SetupAttachment(RootComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(SceneComponent);

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
	TargetSet.Empty();
	ParticleSystem->EndTrails();
}

void AARPG_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARPG_Projectile::AttackTrace()
{
	TArray<FHitResult> OutHits;
	const FVector BeginSocketLocation = StaticMeshComponent->GetSocketLocation(FName("B"));
	const FVector EndSocketLocation = StaticMeshComponent->GetSocketLocation(FName("E"));
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
	}

	//#if ENABLE_DRAW_DEBUG
	//	const FColor DrawColor = bHitCheck ? FColor::Green : FColor::Red;
	//	DrawDebugSweptSphere(GetWorld(), BeginSocketLocation, EndSocketLocation, 20.f, DrawColor, false, 1.f);
	//#endif
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
