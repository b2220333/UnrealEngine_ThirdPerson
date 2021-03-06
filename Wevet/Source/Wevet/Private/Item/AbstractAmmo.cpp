// Copyright 2018 wevet works All Rights Reserved.


#include "Item/AbstractAmmo.h"
#include "Character/CharacterBase.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "WevetExtension.h"


AAbstractAmmo::AAbstractAmmo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AAbstractAmmo::BeginPlay()
{
	Super::BeginPlay();
}

void AAbstractAmmo::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AAbstractAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		const FVector StartLocation = GetActorLocation();
		const FVector TargetLocation = Target->GetActorLocation();
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		WidgetComponent->SetWorldRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));
	}
}


void AAbstractAmmo::OverlapActor(AActor* OtherActor)
{
	Target = OtherActor;
	Super::SetActorTickEnabled(Target ? true : false);
	WidgetComponent->SetVisibility(Target ? true : false);
}

