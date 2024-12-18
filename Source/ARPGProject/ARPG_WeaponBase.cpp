#include "ARPG_WeaponBase.h"

#include "ARPGProject.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystemComponent.h"

struct FAttackData
{
		
};

AARPG_WeaponBase::AARPG_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Pivot");
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(SceneComponent);

	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(StaticMeshComponent);
	ParticleSystem->SetTemplate(TrailParticle);

}

void AARPG_WeaponBase::AttackCheckBegin()
{
	IsAttackCheck = true;
	ParticleSystem->BeginTrails(FName("B"), FName("E"), ETrailWidthMode_FromCentre, 2.f);
}

void AARPG_WeaponBase::AttackCheckEnd()
{
	IsAttackCheck = false;
	TargetSet.Empty();
	ParticleSystem->EndTrails();
}

void AARPG_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARPG_WeaponBase::AttackTrace()
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

		if(OutHit.GetActor()->ActorHasTag(TargetTag))
		{
			FARPG_DamageEvent DamageEventBase;
			DamageEventBase.HitInfo = OutHit;

			OutHit.GetActor()->TakeDamage(10, DamageEventBase, GetOwner()->GetInstigatorController(), this);
		}
	}

//#if ENABLE_DRAW_DEBUG
//	const FColor DrawColor = bHitCheck ? FColor::Green : FColor::Red;
//	DrawDebugSweptSphere(GetWorld(), BeginSocketLocation, EndSocketLocation, 20.f, DrawColor, false, 1.f);
//#endif
}

void AARPG_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsAttackCheck)
	{
		AttackTrace();
	}

}

