// Copyright © 2018 wevet works All Rights Reserved.

#include "AI/AISoldier.h"
#include "AI/AIControllerBase.h"
#include "Player/MockCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Lib/WevetBlueprintFunctionLibrary.h"


AAISoldier::AAISoldier(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


bool AAISoldier::CanMeleeStrike_Implementation() const
{
	if (ALSMovementMode != ELSMovementMode::Grounded)
	{
		return false;
	}

	return Super::CanMeleeStrike_Implementation();
}

void AAISoldier::DoFirePressed_Implementation()
{
	if (WasTakeDamagePlaying())
	{
		return;
	}

	if (!Super::HasEquipWeapon())
	{
		Super::EquipmentActionMontage();
		return;
	}

#if false
	if (WasMeleeAttacked())
	{
		MeleeAttackTimeOut -= GetWorld()->GetDeltaSeconds();
		BulletInterval += GetWorld()->GetDeltaSeconds();

		if (BulletInterval >= BulletDelay)
		{
			Super::DoFirePressed_Implementation();
			BulletInterval = ZERO_VALUE;
		}
	}
	else
	{
		bAttackInitialized = false;
	}
#endif

	Super::DoFirePressed_Implementation();
}

void AAISoldier::TakeDamageMontage(const bool InForcePlaying)
{
	Super::DoFireReleassed_Implementation();
	Super::TakeDamageMontage(true);
}

