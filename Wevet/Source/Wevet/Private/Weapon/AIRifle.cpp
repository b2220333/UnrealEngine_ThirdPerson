// Copyright © 2018 wevet works All Rights Reserved.

#include "Weapon/AIRifle.h"
#include "Character/CharacterBase.h"
#include "AI/AICharacterBase.h"
#include "Weapon/BulletBase.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAIRifle::AAIRifle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAIRifle::OnFirePressInternal()
{
	UWorld* const World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	if (!CanMeleeStrike_Implementation())
	{
		return;
	}

	if (Super::bReload)
	{
		return;
	}

	if (Super::WeaponItemInfo.CurrentAmmo <= 0)
	{
		UE_LOG(LogWevetClient, Log, TEXT("Out Of Ammos"));
		Super::DoReload_Implementation();
		return;
	}

	const FVector StartLocation   = GetPointer()->BulletTraceRelativeLocation();
	//const FVector ForwardLocation = GetPointer()->BulletTraceForwardLocation();
	const FVector ForwardLocation = GetPointer()->GetActorForwardVector();
	const FVector EndLocation = StartLocation + (ForwardLocation * WeaponItemInfo.TraceDistance);

	FHitResult HitData(ForceInit);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.TraceTag = FName("");
	CollisionQueryParams.OwnerTag = FName("");
	CollisionQueryParams.bTraceComplex = true;
	CollisionQueryParams.bFindInitialOverlaps = false;
	CollisionQueryParams.bReturnFaceIndex = false;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	CollisionQueryParams.bIgnoreBlocks = false;
	CollisionQueryParams.IgnoreMask = 0;
	CollisionQueryParams.AddIgnoredActors(IgnoreActors);

	const bool bSuccess = World->LineTraceSingleByChannel(
		HitData,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		CollisionQueryParams);

	ISoundInstigator::Execute_ReportNoiseOther(GetPointer(), this, FireSound, DEFAULT_VOLUME, StartLocation);
	GetPointer()->FireActionMontage();
	--WeaponItemInfo.CurrentAmmo;

	ISoundInstigator::Execute_ReportNoiseOther(GetPointer(), this, ImpactSound, DEFAULT_VOLUME, HitData.Location);
	const FVector EndPoint   = UKismetMathLibrary::SelectVector(HitData.ImpactPoint, HitData.TraceEnd, bSuccess);
	const FRotator Rotation  = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndPoint);
	FTransform Transform = UKismetMathLibrary::MakeTransform(StartLocation, Rotation, FVector::OneVector);

	// debug
	//UKismetSystemLibrary::DrawDebugLine(World, StartLocation, EndPoint, FLinearColor(255, 0, 0, 0), 1.0f, 1.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	ABulletBase* const Bullet = World->SpawnActor<ABulletBase>(Super::BulletsTemplate, Transform, SpawnParams);

#if WITH_EDITOR
	Bullet->SetFolderPath("/BulletsRoot");
#endif

	bool bCanDamageResult = false;
	Super::CheckIfValid(HitData.GetActor(), bCanDamageResult);
	if (bCanDamageResult)
	{
		Super::TakeDamageOuter(HitData.GetActor(), HitData.BoneName, WeaponItemInfo.Damage);
	}
	Super::PlayEffect(HitData, SkeletalMeshComponent);
}
