// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Pistol.h"
#include "AIPistol.generated.h"

/**
 * 
 */
UCLASS()
class WEVET_API AAIPistol : public APistol
{
	GENERATED_BODY()
	
public:
	AAIPistol(const FObjectInitializer& ObjectInitializer);
	virtual void OnFirePressInternal() override;
};
