#include "ARPG_MeleeCombatComponent.h"

#include "ARPGProject/ARPG_GameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UARPG_MeleeCombatComponent::UARPG_MeleeCombatComponent()
{
	bComboState = false;
	bMontagePlaying = false;
	NextComboMontage = nullptr;
	StartComboMontage = nullptr;
	AnimInstance = nullptr;
}

void UARPG_MeleeCombatComponent::SetMontageData(const FARPG_MontageData& InMontageData)
{
	MontageData = InMontageData;
}

void UARPG_MeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	const ACharacter* Char = Cast<ACharacter>(GetOwner());
	AnimInstance = Char->GetMesh()->GetAnimInstance();
	if(AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->OnMontageEnded.AddDynamic(this, &UARPG_MeleeCombatComponent::OnMontageEnded);
	AnimInstance->OnMontageBlendingOut.AddDynamic(this, &UARPG_MeleeCombatComponent::OnMontageBlendOut);
}

void UARPG_MeleeCombatComponent::InputAttack()
{
	if (bMontagePlaying)
	{
		if (bComboState)
		{
			PlayMontage(NextComboMontage);
		}
	}
	else
	{
		PlayMontage(StartComboMontage);
	}
}

void UARPG_MeleeCombatComponent::HeavyAttack(const UAnimMontage* NewHeavyAttackMontage)
{
	if (bMontagePlaying)
	{
		return;
	}
	PlayMontage(NewHeavyAttackMontage);
}

void UARPG_MeleeCombatComponent::HeavyAttackComplete(const UAnimMontage* NewHeavyAttackMontage)
{
	PlayMontage(NewHeavyAttackMontage, "E");
}

void UARPG_MeleeCombatComponent::SetNextCombo(const UAnimMontage* NewNextComboMontage)
{
	NextComboMontage = NewNextComboMontage;
	bComboState = NextComboMontage != nullptr;
}

void UARPG_MeleeCombatComponent::SetStartComboMontage(UAnimMontage* InMontage)
{
	StartComboMontage = InMontage;
}

void UARPG_MeleeCombatComponent::Guard()
{
	bIsGuard = true;
	ParryStart();
}

void UARPG_MeleeCombatComponent::GuardComplete()
{
	bIsGuard = false;
	bIsParry = false;
	GetWorld()->GetTimerManager().ClearTimer(ParryTimerHandle);
}

bool UARPG_MeleeCombatComponent::IsGuard() const
{
	return bIsGuard;
}

void UARPG_MeleeCombatComponent::ParryStart()
{
	bIsParry = true;
	GetWorld()->GetTimerManager().SetTimer(ParryTimerHandle, this, &UARPG_MeleeCombatComponent::ParryEnd, ParryTime, false);
}

void UARPG_MeleeCombatComponent::ParryEnd()
{
	bIsParry = false;
}

bool UARPG_MeleeCombatComponent::IsParry() const
{
	return bIsParry;
}

void UARPG_MeleeCombatComponent::PlayMontage(const UAnimMontage* Montage, const FName NextSection)
{
	if (Montage == nullptr)
	{
		return;
	}

	if(AnimInstance == nullptr)
	{
		return;
	}
	
	if (NextSection.IsNone() == false)
	{
		AnimInstance->Montage_SetNextSection("L", NextSection, Montage);
	}
	else
	{
		bMontagePlaying = true;
		AnimInstance->Montage_Play(const_cast<UAnimMontage*>(Montage));
	}
}

void UARPG_MeleeCombatComponent::StopMontage() const
{
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0);
}

void UARPG_MeleeCombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Montage End : %s - %s"), bInterrupted ? TEXT("true") : TEXT("false"), *GetOwner()->GetActorNameOrLabel()));
	OnMontageCancelDelegate.Broadcast();
	if (bInterrupted)
	{
		return;
	}
	bMontagePlaying = false;
	OnMontageEndDelegate.Broadcast();
}

void UARPG_MeleeCombatComponent::OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Montage BlendOut : %s - %s"), bInterrupted ? TEXT("true") : TEXT("false"), *GetOwner()->GetActorNameOrLabel()));

	bMontagePlaying = false;
	
}

bool UARPG_MeleeCombatComponent::IsMontagePlaying() const
{
	return bMontagePlaying;
}

