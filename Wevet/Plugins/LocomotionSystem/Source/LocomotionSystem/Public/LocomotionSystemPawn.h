// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Camera/CameraShake.h"
#include "LocomotionSystemTypes.h"
#include "LocomotionSystemStructs.h"
#include "LocomotionSystemPawn.generated.h"

UINTERFACE(BlueprintType)
class ULocomotionSystemPawn : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class LOCOMOTIONSYSTEM_API ILocomotionSystemPawn
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSCharacterRotation(const FRotator AddAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSCameraShake(TSubclassOf<class UMatineeCameraShake> InShakeClass, const float InScale);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSMovementMode(const ELSMovementMode NewALSMovementMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void OnALSMovementModeChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSMovementAction(const ELSMovementAction NewALSMovementAction);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void OnALSMovementActionChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSGait(const ELSGait NewALSGait);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void OnALSGaitChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSStance(const ELSStance NewALSStance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void OnALSStanceChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSRotationMode(const ELSRotationMode NewALSRotationMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void OnALSRotationModeChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSViewMode(const ELSViewMode NewALSViewMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void OnALSViewModeChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSAiming(const bool NewALSAiming);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void OnALSAimingChange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetALSIdleState(const ELSIdleEntryState InLSIdleEntryState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	ELSMovementMode GetALSMovementMode() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	ELSMovementAction GetALSMovementAction() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	ELSGait GetALSGait() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	ELSStance GetALSStance() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	ELSViewMode GetALSViewMode() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	ELSRotationMode GetALSRotationMode() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	bool HasMovementInput() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	bool HasAiming() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	FTransform GetPivotTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	FVector GetCameraTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetWalkingSpeed(const float InWalkingSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetRunningSpeed(const float InRunningSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetSprintingSpeed(const float InSprintingSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetCrouchingSpeed(const float InCrouchingSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetSwimmingSpeed(const float InSwimmingSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	float GetWalkingSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	float GetRunningSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	float GetSprintingSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	float GetCrouchingSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	float GetSwimmingSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetGetup(const bool InFaceDown);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void SetRF(const bool InRF);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	void PoseSnapShot(const FName InPoseName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	FCameraFOVParam GetCameraFOVParam() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	FCameraTraceParam GetCameraTraceParam() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS_Pawn")
	bool HasDebugTrace() const;
};
