// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/WevetSaveGame.h"
#include "WevetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WEVET_API UWevetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWevetGameInstance();

	virtual void Init() override
	{
		Super::Init();
		Instance = this;
	}

	virtual void BeginDestroy() override
	{
		Instance = nullptr;
		Super::BeginDestroy();
	}

	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadOrCreateSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	void ResetSaveGame(bool& bClearSuccess);

protected:
	void SetSavingEnabled(const bool bEnabled)
	{
		bSavingEnabled = bEnabled;
	}

	UPROPERTY(BlueprintReadWrite, Category = Save)
	FString SaveSlot;

	UPROPERTY(BlueprintReadWrite, Category = Save)
	int32 SaveUserIndex;

	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame() const
	{
		if (bSavingEnabled)
		{
			return UGameplayStatics::SaveGameToSlot(GetSaveGame(), SaveSlot, SaveUserIndex);
		}
		return false;
	}

	UPROPERTY()
	bool bSavingEnabled;

	UPROPERTY()
	UWevetSaveGame* CurrentSaveGame;

public:
	static UWevetGameInstance* GetInstance();

	UFUNCTION(BlueprintCallable, Category = Save)
	UWevetSaveGame* GetSaveGame() const
	{
		return CurrentSaveGame;
	}

private:
	static UWevetGameInstance* Instance;

	void AddDefaultInventory(UWevetSaveGame* SaveGame, const bool bRemoveExtra = false);
};
