// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ARPG_AN_DeathEvent.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPROJECT_API UARPG_AN_DeathEvent : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
