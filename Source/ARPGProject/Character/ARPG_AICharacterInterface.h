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
	UFUNCTION(BlueprintNativeEvent)
	void SetWalkSpeed(const float InSpeed);
	virtual void SetWalkSpeed_Implementation(const float InSpeed) = 0;
};
