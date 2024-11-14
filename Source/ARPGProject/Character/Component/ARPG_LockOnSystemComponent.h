#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPG_LockOnSystemComponent.generated.h"

class AARPG_EnemyCharacter;
class AARPG_Character;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARPGPROJECT_API UARPG_LockOnSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARPG_LockOnSystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void InputTargetLockOn();
	bool IsLockOnTarget() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AARPG_EnemyCharacter* TargetCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AARPG_Character* OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float SphereRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float MaxLockOnAngle = 90.f;

public:
	AActor* FindForwardClosestTarget() const;
	AActor* FindClosestTarget() const;
	void SetTarget(AActor* NewTargetActor);
	void SetMovementLockOnTarget() const;
};
