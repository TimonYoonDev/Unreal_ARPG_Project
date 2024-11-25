#pragma once

#include "CoreMinimal.h"
#include "ARPG_AICharacter.h"
#include "ARPG_NoWeaponEnemyCharacter.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_NoWeaponEnemyCharacter : public AARPG_AICharacter
{
	GENERATED_BODY()

	bool bIsAttacking = false;
	TSet<FString> TargetSet;

public:
	AARPG_NoWeaponEnemyCharacter();



protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	virtual void AttackCheckBegin() override;
	virtual void AttackCheckEnd() override;

	void AttackTrace();

	
};
