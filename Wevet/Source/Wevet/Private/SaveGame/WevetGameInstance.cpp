// Copyright 2018 wevet works All Rights Reserved.


#include "SaveGame/WevetGameInstance.h"


UWevetGameInstance::UWevetGameInstance()
	: SaveSlot(TEXT("SaveGame")),
	SaveUserIndex(0)
{
	//
}

UWevetGameInstance* UWevetGameInstance::Instance = nullptr;

UWevetGameInstance* UWevetGameInstance::GetInstance()
{
	return Instance;
}

void UWevetGameInstance::ResetSaveGame(bool& bClearSuccess)
{
	bClearSuccess = UGameplayStatics::DeleteGameInSlot(SaveSlot, SaveUserIndex);
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	bClearSuccess = LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}

void UWevetGameInstance::AddDefaultInventory(UWevetSaveGame* SaveGame, const bool bRemoveExtra)
{
	if (bRemoveExtra)
	{

	}

}

bool UWevetGameInstance::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		CurrentSaveGame = Cast<UWevetSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		AddDefaultInventory(CurrentSaveGame, false);
		return true;
	}
	else
	{
		CurrentSaveGame = Cast<UWevetSaveGame>(UGameplayStatics::CreateSaveGameObject(UWevetSaveGame::StaticClass()));
		AddDefaultInventory(CurrentSaveGame, true);
		return false;
	}
}
