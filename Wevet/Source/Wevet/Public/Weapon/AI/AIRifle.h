// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Rifle.h"
#include "AIRifle.generated.h"

/**
 * 
 */
UCLASS()
class WEVET_API AAIRifle : public ARifle
{
	GENERATED_BODY()
	
public:
	AAIRifle(const FObjectInitializer& ObjectInitializer);
	virtual void OnFirePressInternal() override;
};
