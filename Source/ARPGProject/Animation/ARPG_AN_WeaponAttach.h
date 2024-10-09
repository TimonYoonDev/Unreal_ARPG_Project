// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ARPG_AN_WeaponAttach.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_AN_WeaponAttach : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FName AttachSocketName;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
