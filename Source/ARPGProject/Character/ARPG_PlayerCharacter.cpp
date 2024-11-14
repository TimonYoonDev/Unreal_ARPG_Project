// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PlayerCharacter.h"

#include "ARPGProject/ARPG_InteractableInterface.h"

AARPG_PlayerCharacter::AARPG_PlayerCharacter()
{
}

void AARPG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AARPG_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    InteractWithObject();
}

void AARPG_PlayerCharacter::InteractWithObject()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * 500.f;

	const FCollisionQueryParams CollisionParams;
    if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
    {
        if (IARPG_InteractableInterface* InteractableObject = Cast<IARPG_InteractableInterface>(HitResult.GetActor()))
        {
            InteractableObject->Interact(this);
        }
    }

    DrawDebugLine(GetWorld(), Start, End, FColor::Green);
}
