// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARPG_WeaponBase.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AARPG_WeaponBase();

	void AttackCheckBegin();
	void AttackCheckEnd();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	UParticleSystem* TrailParticle;

protected:
	// Called when the game starts or when spawned
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
