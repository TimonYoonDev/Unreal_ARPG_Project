// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeCombatComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMeleeCombatComponent::UMeleeCombatComponent()
{
	bComboState = false;
	bMontagePlaying = false;
	NextComboMontage = nullptr;
	StartComboMontage = nullptr;
	AnimInstance = nullptr;
}

void UMeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	const ACharacter* Char = Cast<ACharacter>(GetOwner());
	AnimInstance = Char->GetMesh()->GetAnimInstance();
	AnimInstance->OnMontageEnded.AddDynamic(this, &UMeleeCombatComponent::OnMontageEnded);
}

void UMeleeCombatComponent::InputAttack()
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

void UMeleeCombatComponent::HeavyAttack(const UAnimMontage* NewHeavyAttackMontage)
{
	if (bMontagePlaying)
	{
		return;
	}
	PlayMontage(NewHeavyAttackMontage);
}

void UMeleeCombatComponent::HeavyAttackComplete(const UAnimMontage* NewHeavyAttackMontage)
{
	PlayMontage(NewHeavyAttackMontage, "E");
}

void UMeleeCombatComponent::SetNextCombo(const UAnimMontage* NewNextComboMontage)
{
	NextComboMontage = NewNextComboMontage;
	bComboState = NextComboMontage != nullptr;
}

void UMeleeCombatComponent::SetStartComboMontage(UAnimMontage* InMontage)
{
	StartComboMontage = InMontage;
}

void UMeleeCombatComponent::PlayMontage(const UAnimMontage* Montage, const FName NextSection)
{
	if (Montage == nullptr)
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

void UMeleeCombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Montage End : %s"), bInterrupted ? TEXT("true") : TEXT("false")));
	if (bInterrupted)
	{
		// 강제 종료되면 반환
		return;
	}
	bMontagePlaying = false;
}

bool UMeleeCombatComponent::GetMontagePlaying() const
{
	return bMontagePlaying;
}

