// Copyright © 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STypes.h"
#include "WeaponControllerExecuter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemBase.generated.h"

class ACharacterBase;

UCLASS(ABSTRACT)
class WEVET_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponent;

	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	USoundBase* PickupSoundAsset;

public:
	FORCEINLINE class UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return StaticMeshComponent;
	}

	FORCEINLINE class USphereComponent* GetSphereComponent() const
	{
		return SphereComponent;
	}

	FORCEINLINE class UWidgetComponent* GetWidgetComponent() const
	{
		return WidgetComponent;
	}

	UFUNCTION(BlueprintCallable, Category = Components)
	virtual	void BeginOverlapRecieve(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable, Category = Components)
	virtual	void EndOverlapRecieve(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void TakeResult();

	const EItemType GetItemType() { return ItemType; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Variable")
	FWeaponItemInfo WeaponItemInfo;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|Variable")

};
