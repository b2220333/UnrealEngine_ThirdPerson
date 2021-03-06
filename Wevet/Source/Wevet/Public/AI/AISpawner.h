// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AICharacterBase.h"
#include "AISpawner.generated.h"

UCLASS()
class WEVET_API AAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawner(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISpawner|Variable")
	int32 SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISpawner|Variable")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISpawner|Variable")
	TSubclassOf<class AAICharacterBase> CharacterTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AISpawner|Variable")
	float SpawnRadius;

public:
	UFUNCTION(BlueprintCallable, Category = "AISpawner|Function")
	void StartSpawn();

protected:
	void DoSpawn();

protected:
	UPROPERTY()
	int32 CurrentSpawnCount;

	UPROPERTY()
	float SpawnTimer;

	UPROPERTY()
	TArray<FVector> SpawnPoints;

	bool bSpawnFinished;
};
