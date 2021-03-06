// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterActor.generated.h"

class ACharacterBase;

UENUM()
enum class EWaterType : uint8
{
	None,
	Shallow,
	Deep,
};

UCLASS()
class WEVET_API AWaterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaterActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	class ACharacterBase* Target;

	UFUNCTION(BlueprintCallable, Category = "Water|Function")
	void SetHitData(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Water|Function")
	void ForceUnEquip(class ACharacterBase* InTarget);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Water|Variable")
	bool bInWater;

	FHitResult CurrentHitData;
	EWaterType WaterType;
	FVector HitPosition;
};
