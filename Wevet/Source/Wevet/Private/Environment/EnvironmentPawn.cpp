// Copyright © 2018 wevet works All Rights Reserved.

#include "EnvironmentPawn.h"

AEnvironmentPawn::AEnvironmentPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	StaticMeshComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->bRenderCustomDepth = true;
	StaticMeshComponent->CustomDepthStencilValue = 1;
}

void AEnvironmentPawn::BeginPlay()
{
	Super::BeginPlay();	
}

void AEnvironmentPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void AEnvironmentPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

