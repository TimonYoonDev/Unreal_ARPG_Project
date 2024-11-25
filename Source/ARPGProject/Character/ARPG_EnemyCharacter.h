#pragma once

#include "CoreMinimal.h"
#include "ARPG_AICharacter.h"
#include "ARPG_EnemyCharacter.generated.h"

UCLASS()
class ARPGPROJECT_API AARPG_EnemyCharacter : public AARPG_AICharacter
{
	GENERATED_BODY()

public:
	void WeaponGrip();
};
