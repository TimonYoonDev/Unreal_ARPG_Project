#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPG_LockOnSystemComponent.generated.h"

class AARPG_Character;
class AARPG_AICharacter;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARPGPROJECT_API UARPG_LockOnSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "LockOnInfo")
	AARPG_AICharacter* TargetCharacter;
	UPROPERTY(VisibleAnywhere, Category = "LockOnInfo")
	AARPG_Character* OwnerCharacter;
	UPROPERTY(EditAnywhere, Category = "LockOnInfo")
	float SphereRadius = 500.f;
	UPROPERTY(EditAnywhere, Category = "LockOnInfo")
	float MaxLockOnAngle = 90.f;

public:
	UARPG_LockOnSystemComponent();

	void InputTargetLockOn();
	bool IsLockOnTarget() const;
	AActor* FindForwardClosestTarget() const;
	AActor* FindClosestTarget() const;
	void SetTarget(AActor* NewTargetActor);
	AActor* GetTarget() const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
private:
	void SetMovementLockOnTarget() const;
	
};
