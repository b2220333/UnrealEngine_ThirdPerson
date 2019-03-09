// Copyright © 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentActor.generated.h"

UCLASS()
class WEVET_API AEnvironmentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentActor(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
