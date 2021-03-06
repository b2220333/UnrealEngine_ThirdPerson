// Copyright 2018 wevet works All Rights Reserved.


#include "Singleton/BattleGameManager.h"
#include "Lib/SingletonFunctionLibrary.h"

ABattleGameManager* ABattleGameManager::Instance = nullptr;

ABattleGameManager::ABattleGameManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	BattlePhase = Wevet::BattlePhase::Normal;
}

void ABattleGameManager::BeginPlay()
{
	Super::BeginPlay();
}

void ABattleGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ABattleGameManager* ABattleGameManager::GetInstance()
{
	return USingletonFunctionLibrary::GetSingletonInstance<ABattleGameManager>(Instance);
}

void ABattleGameManager::SetBattlePhase(const Wevet::BattlePhase InBattlePhase)
{
	if (BattlePhase != InBattlePhase)
	{
		BattlePhase = InBattlePhase;
	}
	// 0 => normal
	// 1 => warning
	// 2 => alert
	UE_LOG(LogWevetClient, Log, TEXT("BattlePhase : %d, funcName : %s"), (int)BattlePhase, *FString(__FUNCTION__));
}
