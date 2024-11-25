#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARPG_AICharacterInterface.generated.h"

UINTERFACE(MinimalAPI)
class UARPG_AICharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class ARPGPROJECT_API IARPG_AICharacterInterface
{
	GENERATED_BODY()

public:
	virtual void SetWalkSpeed(const float InSpeed) = 0;

	virtual void SetDefence(const bool bIsDefence) = 0;

	virtual void SetLockOn(const bool bIsLockOn) = 0;


	virtual void SetLockOnWidget(const bool bShowWidget) = 0;
	//virtual void RotateToTarget() = 0;
};
