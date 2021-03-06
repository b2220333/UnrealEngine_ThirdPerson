// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WEVET_API UIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIKAnimInstance(const FObjectInitializer& ObjectInitializer);
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	bool bEnableIK;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	bool bDebugTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	float IKAlpha;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	float PelvisOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FName LeftFootBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FName RightFootBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FVector MinLimitsStanding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FVector MaxLimitsStanding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FVector MinLimitsCrouching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FVector MaxLimitsCrouching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FVector LeftFootOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	FVector RightFootOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	float ZOffsetInterpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	float RotationOffsetInterpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	float TraceLengthAboveFoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	float TraceLengthBelowFoot;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

protected:
	UFUNCTION(BlueprintCallable, Category = "IKAnimInstance")
	FVector ClampOffset(const bool WasCrouching, const float Roll, const float Pitch, const float Z) const;

	UFUNCTION(BlueprintCallable, Category = "IKAnimInstance")
	void TraceOffset(const float X, const float Y, const float Z, FVector& OutStart, FVector& OutEnd);

	UFUNCTION(BlueprintCallable, Category = "IKAnimInstance")
	FVector InterpOffset(const FVector Current, const FVector Target) const;

	UFUNCTION(BlueprintCallable, Category = "IKAnimInstance")
	void UpdateFootIK(const bool WasCrouching, const FName SocketName, FVector& OutTargetPos);

	UFUNCTION(BlueprintCallable, Category = "IKAnimInstance")
	void UpdateIK(const bool WasCrouching);
};
