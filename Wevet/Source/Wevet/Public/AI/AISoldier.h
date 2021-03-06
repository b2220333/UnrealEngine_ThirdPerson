// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacterBase.h"
#include "AISoldier.generated.h"

UCLASS()
class WEVET_API AAISoldier : public AAICharacterBase
{
	GENERATED_BODY()
	
public:
	AAISoldier(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool CanMeleeStrike_Implementation() const override;
	virtual void DoFirePressed_Implementation() override;

protected:
	virtual void TakeDamageMontage(const bool InForcePlaying) override;

};

