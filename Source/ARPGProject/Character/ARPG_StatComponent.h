#pragma once

#include "CoreMinimal.h"
#include "ARPGProject/DataTableStructs.h"
#include "Components/ActorComponent.h"
#include "ARPG_StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate)
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGPROJECT_API UARPG_StatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UARPG_StatComponent();

	FOnHPIsZeroDelegate OnHPIsZeroDelegate;
	FOnHPChangedDelegate OnHPChangedDelegate;
	void SetDamage(float NewDamage);
	float GetDamage() const;

	void SetHP(float NewHP);
	float GetHPRatio() const;
protected:
	FARPG_CharacterData CharacterData;
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
	float CurrentHealth;



		
};
