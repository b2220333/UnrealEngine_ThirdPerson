// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WevetTypes.h"
#include "Structs/BaseItem.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/InteractionItem.h"
#include "AbstractItem.generated.h"

class ACharacterBase;

UCLASS(ABSTRACT)
class WEVET_API AAbstractItem : public AActor, public IInteractionItem
{
	GENERATED_BODY()
	
public:	
	AAbstractItem(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

protected:
	virtual void BeginPlay() override;

public:
#pragma region Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbstractItem|InteractionItem")
	void Take(ACharacterBase* NewCharacter);
	virtual void Take_Implementation(ACharacterBase* NewCharacter) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbstractItem|InteractionItem")
	void Release(ACharacterBase* NewCharacter);
	virtual void Release_Implementation(ACharacterBase* NewCharacter) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbstractItem|InteractionItem")
	EItemType GetItemType() const;
	virtual EItemType GetItemType_Implementation() const override
	{
		return WeaponAmmoInfo.BaseItemType;
	}
#pragma endregion

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbstractItem|Variable")
	FWeaponAmmoInfo WeaponAmmoInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbstractItem|Variable")
	class USoundBase* PickupSound;

	class AActor* Target;

protected:
	UFUNCTION()
	virtual void HitReceive(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void BeginOverlapRecieve(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlapRecieve(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE class UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return StaticMeshComponent; 
	}

	struct FWeaponAmmoInfo GetWeaponAmmoInfo() const 
	{
		return WeaponAmmoInfo; 
	}

	FORCEINLINE EWeaponItemType GetWeaponItemType() const 
	{
		return WeaponAmmoInfo.WeaponItemType; 
	}

	FORCEINLINE int32 GetReplenishmentAmmo() const 
	{
		return WeaponAmmoInfo.Ammo; 
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponent;

protected:
	void AddDelegate();
	void RemoveDelegate();
	void UpdateCustomDepth(const bool bEnable);

	virtual void OverlapActor(AActor* OtherActor) {}
};
