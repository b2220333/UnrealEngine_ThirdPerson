// Copyright © 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacterBase.h"
#include "AISoldier.generated.h"

/**
 * 
 */
UCLASS()
class WEVET_API AAISoldier : public AAICharacterBase
{
	GENERATED_BODY()
	
public:
	AAISoldier(const FObjectInitializer& ObjectInitializer);

public:
	virtual void MainLoop(float DeltaTime) override;
	virtual void OnTakeDamage_Implementation(FName BoneName, float Damage, AActor* Actor) override;

protected:
	//UFUNCTION(BlueprintCallable, Category = "AAISoldier|PawnSensing")
	virtual	void OnSeePawnRecieve(APawn* OtherPawn) override;

	//UFUNCTION(BlueprintCallable, Category = "AAISoldier|PawnSensing")
	virtual	void OnHearNoiseRecieve(APawn *OtherActor, const FVector &Location, float Volume) override;

private:
	float LastWarningTime;
	float SenceWarningTimeOut;
};
