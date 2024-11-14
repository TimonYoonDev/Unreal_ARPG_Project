// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ARPG_Projectile.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AARPG_Projectile();

	void SetVelocity(FVector Velocity);

	void AttackCheckBegin();
	void AttackCheckEnd();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = true))
	UParticleSystem* TrailParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovementComponent;


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	UStaticMesh* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* ParticleSystem;

	bool IsAttackCheck;
	TSet<FString> TargetSet;
	void AttackTrace();

};
