#include "ARPG_CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"

UARPG_CameraComponent::UARPG_CameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OriginCameraInfo = new FARPG_CameraInfo;
	OriginCameraInfo->TargetArmLength = 300.f;
	OriginCameraInfo->SocketOffset = FVector(50, 50, 50);
	OriginCameraInfo->FieldOfView = 90;
	OriginCameraInfo->bEnableCameraLag = true;

	AimCameraInfo = new FARPG_CameraInfo;
	AimCameraInfo->TargetArmLength = 400.f;
	AimCameraInfo->SocketOffset = FVector(100, 100, 50);
	AimCameraInfo->FieldOfView = 45;
	AimCameraInfo->bEnableCameraLag = false;

	FinishAttackCameraInfo = new FARPG_CameraInfo;
	FinishAttackCameraInfo->TargetArmLength = 250.f;
	FinishAttackCameraInfo->SocketOffset = FVector(0, 0, 130);
	FinishAttackCameraInfo->FieldOfView = 90;
	FinishAttackCameraInfo->bEnableCameraLag = false;

	BeginCameraInfo = OriginCameraInfo;
	EndCameraInfo = OriginCameraInfo;
	CurrentCameraInfo = new FARPG_CameraInfo;

}

void UARPG_CameraComponent::Init(USpringArmComponent* InCameraBoom, UCameraComponent* InFollowCamera)
{
	CameraBoom = InCameraBoom;
	FollowCamera = InFollowCamera;
}

void UARPG_CameraComponent::OriginCameraMove()
{
	BeginCameraInfo = CurrentCameraInfo;
	EndCameraInfo = OriginCameraInfo;
	CurrentCameraMovingTime = 0;
	CameraBoom->bDoCollisionTest = true;

}

void UARPG_CameraComponent::AimCameraMove()
{
	BeginCameraInfo = CurrentCameraInfo;
	EndCameraInfo = AimCameraInfo;
	CurrentCameraMovingTime = 0;
	CameraBoom->bDoCollisionTest = false;
}

void UARPG_CameraComponent::FinishAttackCameraMove()
{
	BeginCameraInfo = CurrentCameraInfo;
	EndCameraInfo = FinishAttackCameraInfo;
	CurrentCameraMovingTime = 0;
	CameraBoom->bDoCollisionTest = false;
}

void UARPG_CameraComponent::SetCameraInfo(const FARPG_CameraInfo* CameraInfo) const
{
	if(CameraBoom == nullptr || FollowCamera == nullptr)
	{
		return;;
	}
	CameraBoom->TargetArmLength = CameraInfo->TargetArmLength;
	CameraBoom->SocketOffset = CameraInfo->SocketOffset;
	CameraBoom->bEnableCameraLag = CameraInfo->bEnableCameraLag;
	FollowCamera->FieldOfView = CameraInfo->FieldOfView;
}

void UARPG_CameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentCameraMovingTime < TargetCameraMovingTime)
	{
		if(CurrentCameraInfo == nullptr || BeginCameraInfo == nullptr || EndCameraInfo == nullptr)
		{
			return;
		}
		CurrentCameraMovingTime += DeltaTime;
		const float Alpha = CurrentCameraMovingTime / TargetCameraMovingTime;
		CurrentCameraInfo->TargetArmLength = UKismetMathLibrary::Lerp(BeginCameraInfo->TargetArmLength, EndCameraInfo->TargetArmLength, Alpha);
		CurrentCameraInfo->SocketOffset = UKismetMathLibrary::VLerp(BeginCameraInfo->SocketOffset, EndCameraInfo->SocketOffset, Alpha);
		CurrentCameraInfo->bEnableCameraLag = EndCameraInfo->bEnableCameraLag;
		CurrentCameraInfo->FieldOfView = UKismetMathLibrary::Lerp(BeginCameraInfo->FieldOfView, EndCameraInfo->FieldOfView, Alpha);
		SetCameraInfo(CurrentCameraInfo);
	}
}

