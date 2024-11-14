// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ARPG_CameraComponent.generated.h"


struct FARPG_CameraInfo
{
	float TargetArmLength;
	FVector SocketOffset;
	bool bEnableCameraLag;

	float FieldOfView;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPGPROJECT_API UARPG_CameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UARPG_CameraComponent();

	void Init(USpringArmComponent* InCameraBoom, UCameraComponent* InFollowCamera);
	void OriginCameraMove();
	void AimCameraMove();
	void FinishAttackCameraMove();


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	float TargetCameraMovingTime = 1.f;
	float CurrentCameraMovingTime = 0;

	FARPG_CameraInfo* OriginCameraInfo;
	FARPG_CameraInfo* AimCameraInfo;
	FARPG_CameraInfo* FinishAttackCameraInfo;

	FARPG_CameraInfo* BeginCameraInfo;
	FARPG_CameraInfo* EndCameraInfo;
	FARPG_CameraInfo* CurrentCameraInfo;

	void SetCameraInfo(const FARPG_CameraInfo* CameraInfo) const;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
