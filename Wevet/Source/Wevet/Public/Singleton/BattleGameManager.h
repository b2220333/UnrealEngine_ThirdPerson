// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WevetTypes.h"
#include "BattleGameManager.generated.h"

UCLASS()
class WEVET_API ABattleGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABattleGameManager(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	static ABattleGameManager* GetInstance();

	void SetBattlePhase(const Wevet::BattlePhase InBattlePhase);

private:
	static ABattleGameManager* Instance;
	Wevet::BattlePhase BattlePhase;
};
