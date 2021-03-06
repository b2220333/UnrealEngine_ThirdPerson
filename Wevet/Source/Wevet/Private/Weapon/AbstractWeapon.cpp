// Copyright 2018 wevet works All Rights Reserved.


#include "Weapon/AbstractWeapon.h"
#include "Weapon/BulletBase.h"
#include "Character/CharacterModel.h"
#include "Character/CharacterBase.h"

#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Wevet.h"
#include "WevetExtension.h"

#include "Interface/AIPawnOwner.h"
#include "Lib/WevetBlueprintFunctionLibrary.h"
#include "Perception/AISense_Hearing.h"

AAbstractWeapon::AAbstractWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	MuzzleSocketName(FName(TEXT("MuzzleFlash"))),
	GripSocketName(FName(TEXT("Grip")))
{
	PrimaryActorTick.bCanEverTick = true;
	bEquip  = false;
	bReload = false;
	bDebugTrace = false;
	bWasMeleeAttack = false;
	bCanFire = false;
	GiveDamageType = EGiveDamageType::None;

	SkeletalMeshComponent = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;
	SkeletalMeshComponent->SetSimulatePhysics(false);
	SkeletalMeshComponent->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::SimulationUpatesComponentTransform;
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	SkeletalMeshComponent->SetEnableGravity(false);

	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);

	MeleeCollisionComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("MeleeCollision"));
	MeleeCollisionComponent->SetupAttachment(RootComponent);
	MeleeCollisionComponent->SetNotifyRigidBodyCollision(true);
	MeleeCollisionComponent->SetGenerateOverlapEvents(false);
	//MeleeCollisionComponent->SetCollisionProfileName(TEXT("WorldDynamic"));
	//MeleeCollisionComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	//MeleeCollisionComponent->SetComponentTickEnabled(false);
	MeleeCollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	{
		static ConstructorHelpers::FObjectFinder<USoundBase> FindAsset(TEXT("/Engine/VREditor/Sounds/UI/Object_PickUp"));
		PickupSound = FindAsset.Object;
	}

}

void AAbstractWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AAbstractWeapon::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAbstractWeapon::BeginPlay()
{
	Super::BeginPlay();
	Super::SetActorTickEnabled(false);
	AddDelegate();
}

void AAbstractWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ResetCharacterOwner();
	PrepareDestroy();

	if (MeleeCollisionComponent)
	{
		MeleeCollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AAbstractWeapon::OnMeleeOverlapRecieve);
	}
	Super::EndPlay(EndPlayReason);
}

void AAbstractWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Public
void AAbstractWeapon::Initialize(ACharacterBase* const NewCharacterOwner)
{
	CharacterPtr = MakeWeakObjectPtr<ACharacterBase>(NewCharacterOwner);
	if (GetPawnOwner())
	{
		IgnoreActors.Add(GetPawnOwner());
		IgnoreActors.Add(this);
		Super::SetOwner(GetPawnOwner());
	}
}

ACharacterBase* AAbstractWeapon::GetPointer() const
{
	return Cast<ACharacterBase>(GetPawnOwner());
}

APawn* AAbstractWeapon::GetPawnOwner() const
{
	if (CharacterPtr.IsValid())
	{
		return CharacterPtr.Get();
	}
	return nullptr;
}

void AAbstractWeapon::ReloadBulletParams()
{
	WeaponItemInfo.NeededAmmo = (WeaponItemInfo.ClipType - WeaponItemInfo.CurrentAmmo);
	WeaponItemInfo.Replenishment();
}

void AAbstractWeapon::SetOwnerNoSeeMesh(const bool NewOwnerNoSee)
{
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetOwnerNoSee(NewOwnerNoSee);
	}
}

void AAbstractWeapon::SpawnToWorld()
{
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		SkeletalMeshComponent->SetEnableGravity(true);
		SkeletalMeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
		SkeletalMeshComponent->SetAllBodiesSimulatePhysics(true);
		SkeletalMeshComponent->WakeAllRigidBodies();
	}
}
#pragma endregion

#pragma region Interface
void AAbstractWeapon::DoFirePressed_Implementation() 
{
	bCanFire = true;
}

void AAbstractWeapon::DoFireRelease_Implementation()
{
	bCanFire = false;
}

// Physics MeleeAttack
void AAbstractWeapon::DoMeleeAttack_Implementation(const bool InEnableMeleeAttack)
{
	if (bWasMeleeAttack == InEnableMeleeAttack)
	{
		return;
	}

	bWasMeleeAttack = InEnableMeleeAttack;
	MeleeCollisionComponent->SetGenerateOverlapEvents(bWasMeleeAttack);
	//MeleeCollisionComponent->SetVisibility(bWasMeleeAttack);

	//const float Duration = 4.0f;
	//const FString Str = FString::Printf(TEXT("MeleeAttack => %s"), bWasMeleeAttack ? TEXT("begin") : TEXT("end"));
	//UWevetBlueprintFunctionLibrary::DrawDebugString(GetPawnOwner(), Str, FLinearColor::Green, Duration);
}

// Reload
void AAbstractWeapon::DoReload_Implementation()
{
	if (bReload)
	{
		return;
	}

	if (WeaponItemInfo.MaxAmmo <= 0)
	{
		UE_LOG(LogWevetClient, Log, TEXT("Empty Ammos Current:%d, ClipType:%d"), WeaponItemInfo.CurrentAmmo, WeaponItemInfo.ClipType);
		return;
	}

	if (WeaponItemInfo.CurrentAmmo >= WeaponItemInfo.ClipType)
	{
		UE_LOG(LogWevetClient, Log, TEXT("Full Ammos Current:%d, ClipType:%d"), WeaponItemInfo.CurrentAmmo, WeaponItemInfo.ClipType);
		return;
	}

	bCanFire = false;
	SetReload(true);
	UWevetBlueprintFunctionLibrary::DrawDebugString(GetPawnOwner(), FString(TEXT("Start Reload")));
	ReloadDuration = DEFAULT_VALUE;
	CharacterPtr.Get()->ReloadActionMontage(ReloadDuration);
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([&]
	{
		SetReload(false);
		UWevetBlueprintFunctionLibrary::DrawDebugString(GetPawnOwner(), FString(TEXT("End Reload")));
	});
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, TimerCallback, ReloadDuration, false);
}

void AAbstractWeapon::DoReplenishment_Implementation(const int32 InAddAmmo)
{
	WeaponItemInfo.MaxAmmo += InAddAmmo;
	WeaponItemInfo.Replenishment();
}

bool AAbstractWeapon::CanMeleeStrike_Implementation() const
{
	//unsafe pointer
	if (!CharacterPtr.IsValid())
	{
		return false;
	}

	// was deaded owner
	if (IDamageInstigator::Execute_IsDeath(CharacterPtr.Get()))
	{
		return false;
	}

	if (!bEquip)
	{
		UE_LOG(LogWevetClient, Log, TEXT("No Equip => %s"), *FString(__FUNCTION__));
		return false;
	}

	if (bReload)
	{
		UE_LOG(LogWevetClient, Log, TEXT("Reload => %s"), *FString(__FUNCTION__));
		return false;
	}

	if (bWasMeleeAttack)
	{
		UE_LOG(LogWevetClient, Log, TEXT("MeleeAttack => %s"), *FString(__FUNCTION__));
		return false;
	}

	if (WasEmpty())
	{
		UE_LOG(LogWevetClient, Log, TEXT("EmptyAmmos => %s"), *FString(__FUNCTION__));
		return false;
	}

	return true;
}

// TakeWeapon
void AAbstractWeapon::Take_Implementation(ACharacterBase* NewCharacter)
{
	ResetCharacterOwner();
	Initialize(NewCharacter);

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	}

	UpdateCustomDepth(false);
	RemoveDelegate();
	if (CollisionComponent)
	{
		CollisionComponent->ConditionalBeginDestroy();
		CollisionComponent = nullptr;
	}
}

// Release Weapon
void AAbstractWeapon::Release_Implementation(ACharacterBase* NewCharacter)
{
	ResetCharacterOwner();
	SetEquip(false);

	// If Player Postprocess is enabled, disable it
	if (NewCharacter)
	{
		NewCharacter->OverlapActor(nullptr);
	}

	if (IsValidLowLevel())
	{
		PrepareDestroy();
		Super::Destroy();
		Super::ConditionalBeginDestroy();
	}	
}
#pragma endregion

void AAbstractWeapon::ResetCharacterOwner()
{
	CharacterPtr.Reset();
	IgnoreActors.Reset(0);
	Super::SetOwner(nullptr);
}

void AAbstractWeapon::OnFirePressInternal()
{
	if (!CanMeleeStrike_Implementation())
	{
		return;
	}

	if (!bCanFire)
	{
		return;
	}

	if (WeaponItemInfo.CurrentAmmo <= 0)
	{
		DoReload_Implementation();
		return;
	}

	const FVector StartLocation = GetPointer()->BulletTraceRelativeLocation();
	const FVector EndLocation = GetPointer()->BulletTraceForwardLocation();

	FHitResult HitData(ForceInit);
	FCollisionQueryParams CollisionQueryParams;
	CreateCollisionQueryParams(CollisionQueryParams);

	const bool bSuccess = GetWorld()->LineTraceSingleByChannel(
		HitData,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		CollisionQueryParams);

	ISoundInstigator::Execute_ReportNoiseOther(GetPointer(), this, FireSound, DEFAULT_VOLUME, GetMuzzleTransform().GetLocation());
	GetPointer()->FireActionMontage();
	--WeaponItemInfo.CurrentAmmo;

	const FVector StartPoint = GetMuzzleTransform().GetLocation();
	const FVector EndPoint = UKismetMathLibrary::SelectVector(HitData.ImpactPoint, HitData.TraceEnd, bSuccess);
	const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(StartPoint, EndPoint);
	const FTransform Transform = UKismetMathLibrary::MakeTransform(StartPoint, Rotation, FVector::OneVector);
	ISoundInstigator::Execute_ReportNoiseOther(GetPointer(), this, ImpactSound, DEFAULT_VOLUME, EndPoint);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), EndPoint, DEFAULT_VOLUME, this, WeaponItemInfo.HearingRange);


	if (bDebugTrace)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FLinearColor::Green, 2.0f, 0.0f);
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), EndLocation, 20.f, 12, FLinearColor::Green, 1.0f, 0.0f);
	}

	if (bSuccess)
	{
		if (HitData.GetActor())
		{
			UE_LOG(LogWevetClient, Log, TEXT("HitActor : %s"), *HitData.GetActor()->GetName());
		}
	}

	OnHitTriggerAction(Transform, HitData, EndPoint, bSuccess);
}

#pragma region HitEvent
void AAbstractWeapon::BeginOverlapRecieve(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IInteractionPawn* Interface = Cast<IInteractionPawn>(OtherActor))
	{
		if (!IInteractionPawn::Execute_CanPickup(Interface->_getUObject()))
		{
			return;
		}
	}

	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		UpdateCustomDepth(true);
		Character->OverlapActor(this);
	}
}

void AAbstractWeapon::EndOverlapRecieve(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IInteractionPawn* Interface = Cast<IInteractionPawn>(OtherActor))
	{
		if (!IInteractionPawn::Execute_CanPickup(Interface->_getUObject()))
		{
			return;
		}
	}

	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		UpdateCustomDepth(false);
		Character->OverlapActor(nullptr);
	}
}

void AAbstractWeapon::OnMeleeOverlapRecieve(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CheckIfValid(SweepResult.GetActor()))
	{
		GiveDamageType = EGiveDamageType::Melee;
		TakeDamageOuter(SweepResult);

#if WITH_EDITOR
		const float Duration = 4.0f;
		const FString Str = FString::Printf(TEXT("MeleeAttack => %s"), *SweepResult.GetActor()->GetName());
		UWevetBlueprintFunctionLibrary::DrawDebugString(GetPawnOwner(), Str, FLinearColor::Green, Duration);
#endif
	}
}
#pragma endregion

void AAbstractWeapon::OnHitTriggerAction(const FTransform InTransform, const FHitResult HitResult, const FVector ImpactLocation, const bool bHitSuccess)
{
	if (!BulletsTemplate)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetPawnOwner();
	ABulletBase* const Bullet = GetWorld()->SpawnActor<ABulletBase>(BulletsTemplate, InTransform, SpawnParams);

	if (!Bullet)
	{
		return;
	}

#if WITH_EDITOR
	Bullet->SetFolderPath("/Bullet");
#endif

	//Bullet->VisibleEmitter(!bHitSuccess);
	Bullet->SetOwners(IgnoreActors);

	bool bCanDamageResult = CheckIfValid(HitResult.GetActor());

	if (bCanDamageResult)
	{
		GiveDamageType = EGiveDamageType::Shoot;
		TakeDamageOuter(HitResult);
	}

	const float BulletHearingRange = 500.f;

	// Bullet HitSound
	ISoundInstigator::Execute_ReportNoiseOther(GetPointer(), Bullet, ImpactSound, DEFAULT_VOLUME, ImpactLocation);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), ImpactLocation, DEFAULT_VOLUME, Bullet, BulletHearingRange);
	PlayEffect(HitResult, SkeletalMeshComponent, bCanDamageResult);
}

bool AAbstractWeapon::CheckIfValid(AActor* OtherActor) const
{
	// if unsafe pointer or unsafe owner or same owner & hit actor
	if (OtherActor == nullptr || GetPawnOwner() == nullptr || GetPawnOwner() == OtherActor)
	{
		return false;
	}

	IDamageInstigator* DamageInstigator = Cast<IDamageInstigator>(OtherActor);
	if (DamageInstigator == nullptr || IDamageInstigator::Execute_IsDeath(DamageInstigator->_getUObject()))
	{
		return false;
	}

	// if AIOwner Overlap hit AIOwner
	if (OtherActor->IsA(GetPawnOwner()->GetClass()))
	{
		UE_LOG(LogWevetClient, Warning, TEXT("SameClassess !! \n Owner->%s, Target->%s"), *GetPawnOwner()->GetClass()->GetName(), *OtherActor->GetClass()->GetName());
		return false;
	}
	return true;
}

void AAbstractWeapon::TakeDamageOuter(const FHitResult& HitResult)
{
	AActor* OtherActor = HitResult.GetActor();
	if (OtherActor == nullptr || GetPawnOwner() == nullptr)
	{
		return;
	}

	// Same Actor or Hit Actor Owner or PendingKilling
	if ((OtherActor == this) || (OtherActor == GetPawnOwner()) || OtherActor->IsPendingKill())
	{
		return;
	}

	IDamageInstigator* DamageInstigator = Cast<IDamageInstigator>(OtherActor);
	if (DamageInstigator == nullptr)
	{
		return;
	}

	// @NOTE
	// Head Shot is enabled when the given Damage type is Shoot
	UCharacterModel* const DamageModel = IDamageInstigator::Execute_GetPropertyModel(DamageInstigator->_getUObject());
	float TotalDamage = IDamageInstigator::Execute_MakeDamage(GetPawnOwner(), DamageModel, WeaponItemInfo.Damage);

	// Choose GiveDamageType
	switch (GiveDamageType)
	{
		case EGiveDamageType::Shoot:
		if (IDamageInstigator::Execute_CanKillDealDamage(OtherActor, HitResult.BoneName))
		{
			TotalDamage += (float)DamageModel->GetKillDamage();
		}
		break;
		case EGiveDamageType::Melee:
		{
			const float MeleeWeight = 10.f;
			TotalDamage /= MeleeWeight;
		}
		break;
	}

	
	// @NOTE
	// Blood Effect
	IDamageInstigator::Execute_HitEffectReceive(OtherActor, HitResult, GiveDamageType);

	// @TODO
	TSubclassOf<UDamageType> DamageType;
	UGameplayStatics::ApplyPointDamage(OtherActor, TotalDamage, HitResult.ImpactPoint, HitResult, GetPointer()->GetController(), this, DamageType);
}

void AAbstractWeapon::PlayEffect(const FHitResult& HitResult, USceneComponent* const InComponent, bool& OutHitActor)
{
	UGameplayStatics::SpawnEmitterAttached(
		FlashEmitterTemplate,
		InComponent,
		MuzzleSocketName,
		GetMuzzleTransform().GetLocation(),
		FRotator(GetMuzzleTransform().GetRotation()),
		EAttachLocation::KeepWorldPosition,
		true);

	if (!HitResult.GetActor())
	{
		return;
	}

	if (!HitResult.GetActor()->IsA(ACharacterBase::StaticClass()))
	{
		// @TODO
		// Don't display Impact Emitter on the water surface
		// Implementation Bullet Classes
		if (!HitResult.GetActor()->ActorHasTag(WATER_BODY_TAG))
		{
			FTransform EmitterTransform;
			EmitterTransform.SetIdentity();
			EmitterTransform.SetLocation(HitResult.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEmitterTemplate, EmitterTransform, true);
		}
		OutHitActor = true;
	}
}

void AAbstractWeapon::UpdateCustomDepth(const bool bEnable)
{
	if (SkeletalMeshComponent)
	{
		const int32 Value = bEnable ? (int32)ECustomDepthType::Weapon : (int32)ECustomDepthType::None;
		SkeletalMeshComponent->SetRenderCustomDepth(bEnable);
		SkeletalMeshComponent->SetCustomDepthStencilValue(Value);
	}
}

void AAbstractWeapon::PrepareDestroy()
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.IsTimerActive(ReloadTimerHandle))
	{
		TimerManager.ClearTimer(ReloadTimerHandle);
		TimerManager.ClearAllTimersForObject(this);
	}
}

#pragma region Delegate
void AAbstractWeapon::AddDelegate()
{
	if (CollisionComponent)
	{
		if (!CollisionComponent->OnComponentBeginOverlap.IsBound())
		{
			CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAbstractWeapon::BeginOverlapRecieve);
		}
		if (!CollisionComponent->OnComponentEndOverlap.IsBound())
		{
			CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AAbstractWeapon::EndOverlapRecieve);
		}
	}

	if (MeleeCollisionComponent)
	{
		MeleeCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAbstractWeapon::OnMeleeOverlapRecieve);
	}
}

void AAbstractWeapon::RemoveDelegate()
{
	if (!CollisionComponent)
	{
		return;
	}

	if (CollisionComponent->OnComponentBeginOverlap.IsBound())
	{
		CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AAbstractWeapon::BeginOverlapRecieve);
	}

	if (CollisionComponent->OnComponentEndOverlap.IsBound())
	{
		CollisionComponent->OnComponentEndOverlap.RemoveDynamic(this, &AAbstractWeapon::EndOverlapRecieve);
	}
}
#pragma endregion

void AAbstractWeapon::CreateCollisionQueryParams(FCollisionQueryParams& OutCollisionQueryParams)
{
	OutCollisionQueryParams.TraceTag = DAMAGE_TAG;
	OutCollisionQueryParams.OwnerTag = CHARACTER_TAG;
	OutCollisionQueryParams.bTraceComplex = true;
	OutCollisionQueryParams.bFindInitialOverlaps = false;
	OutCollisionQueryParams.bReturnFaceIndex = false;
	OutCollisionQueryParams.bReturnPhysicalMaterial = false;
	OutCollisionQueryParams.bIgnoreBlocks = false;
	OutCollisionQueryParams.IgnoreMask = 0;
	OutCollisionQueryParams.AddIgnoredActors(IgnoreActors);
}

