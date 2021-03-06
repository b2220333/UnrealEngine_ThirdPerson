// Copyright 2018 wevet works All Rights Reserved.


#include "Player/MockPlayerCameraManager.h"

AMockPlayerCameraManager::AMockPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraBehavior = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CameraBehavior"));
	CameraBehavior->SetupAttachment(GetTransformComponent());
	CameraBehavior->SetHiddenInGame(true);

	ViewPitchMin = -50.f;
	ViewPitchMax = 50.f;
}

void AMockPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

void AMockPlayerCameraManager::OnPossess(APawn* InPawn)
{
	ControlledPawn = InPawn;
	BP_OnPossess();
	UE_LOG(LogWevetClient, Log, TEXT("Possess : %s, funcName : %s"), *ControlledPawn->GetName(), *FString(__FUNCTION__));
}

void AMockPlayerCameraManager::SetLimitPitch(const float InViewPitchMin, const float InViewPitchMax)
{
	//ViewPitchMin = InViewPitchMin;
	//ViewPitchMax = InViewPitchMax;
}

