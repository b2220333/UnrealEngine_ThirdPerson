#include "EnvironmentActor.h"

AEnvironmentActor::AEnvironmentActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();	
}

void AEnvironmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

