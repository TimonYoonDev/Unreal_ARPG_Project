#pragma once

#include "CoreMinimal.h"
#include "ARPG_AICharacterInterface.h"
#include "ARPG_Character.h"
#include "ARPG_AICharacter.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),       // 대기
	Patrol      UMETA(DisplayName = "Patrol"),     // 정찰
	Attack      UMETA(DisplayName = "Attack"),     // 공격
	Death       UMETA(DisplayName = "Death"),      // 죽음
	Alert       UMETA(DisplayName = "Alert"),      // 경계
	Flee        UMETA(DisplayName = "Flee"),       // 도망
	Chase       UMETA(DisplayName = "Chase"),      // 추적
	TakeCover   UMETA(DisplayName = "Take Cover")  // 엄폐
};

class UWidgetComponent;
class UHealthBarWidget;

UCLASS()
class ARPGPROJECT_API AARPG_AICharacter : public AARPG_Character, public IARPG_AICharacterInterface
{
	GENERATED_BODY()

	static const FName LockOnPivotKey;

	bool bIsLockOnTarget = false;
	FVector StartSpawnLocation;


	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

	UPROPERTY()
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;
	UPROPERTY()
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;
	UPROPERTY()
	TObjectPtr<UWidgetComponent> AssassinateWidgetComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* FinishAttackCollider;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* AssassinateCollider;

public:
	AARPG_AICharacter();

	virtual bool IsLockOnTarget() const override;
	virtual void SetLockOnWidget(const bool bShowWidget) override;
	virtual void SetAssassinateWidget(const bool bShowWidget) override;

protected:
	virtual void OnMontageEndCallBack(bool bInterrupted) override;
	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaSeconds) override;
private:
	virtual void SetWalkSpeed(const float InSpeed) override;
	virtual void SetDefence(const bool bIsDefence) override;
	virtual void SetLockOn(const bool bIsLockOn) override;
	virtual FVector GetStartSpawnLocation() override;
	virtual void ParryingReaction() override;
	virtual void FinishAttackReaction() override;
	virtual void AssassinateReaction() override;
	
	void SetWidget();
	void UpdateHealthBar() const;


	UFUNCTION()
	void OnFinishAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnFinishAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnAssassinateOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAssassinateOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};