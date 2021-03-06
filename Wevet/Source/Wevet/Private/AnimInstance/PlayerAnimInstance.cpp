// Copyright 2018 wevet works All Rights Reserved.


#include "AnimInstance/PlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Player = Cast<AMockCharacter>(Owner);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);
}

void UPlayerAnimInstance::SetMoving()
{
	// @NOTE
	// Not Calling Parent Class
}

void UPlayerAnimInstance::SetMovementSpeed()
{
	// @NOTE
	// Not Calling Parent Class
}

void UPlayerAnimInstance::UpdateIKHandTransform()
{
	if (!bIKLeftHandEnabled || !Owner)
	{
		return;
	}

	if (Owner->HasEquipWeapon())
	{
		switch (Owner->GetCurrentWeaponType())
		{
			case EWeaponItemType::Pistol:
			case EWeaponItemType::Rifle:
			case EWeaponItemType::Sniper:
			SetHandTransform(Owner->GetSelectedWeapon()->GetGripTransform(), FTransform::Identity);
			break;

			case EWeaponItemType::Bomb:
			break;
			case EWeaponItemType::Knife:
			break;
		}
	}
	else
	{
		if (bIKLeftHandEnabled)
		{
			Super::SetArmTorsoIKMode(false, false);
		}
	}
}
