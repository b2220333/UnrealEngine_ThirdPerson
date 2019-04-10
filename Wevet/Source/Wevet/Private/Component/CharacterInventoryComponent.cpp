// Copyright © 2018 wevet works All Rights Reserved.

#include "CharacterInventoryComponent.h"
#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "Wevet.h"

using namespace Wevet;

UCharacterInventoryComponent::UCharacterInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	WeaponArray.Reset(0);
	ItemArray.Reset(0);
}

void UCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	WeaponArray.Reset(0);
	ItemArray.Reset(0);
	Super::EndPlay(EndPlayReason);
}

#pragma region weapon
void UCharacterInventoryComponent::AddWeaponInventory(AWeaponBase* const NewWeaponBase)
{
	if (WeaponArray.Find(NewWeaponBase) == INDEX_NONE)
	{
		WeaponArray.Add(NewWeaponBase);
	}
}

void UCharacterInventoryComponent::RemoveWeaponInventory(AWeaponBase* const WeaponBase)
{
	if (WeaponArray.Find(WeaponBase) != INDEX_NONE)
	{
		WeaponArray.Remove(WeaponBase);
	}
}

void UCharacterInventoryComponent::ClearWeaponInventory()
{
	WeaponArray.Empty();
}

const TArray<AWeaponBase*>& UCharacterInventoryComponent::GetWeaponInventory()
{
	return WeaponArray;
}

TArray<AWeaponBase*> UCharacterInventoryComponent::GetWeaponInventoryOriginal()
{
	return WeaponArray;
}

bool UCharacterInventoryComponent::HasInventoryWeaponItems() const
{
	return ArrayExtension::NullOrEmpty(WeaponArray);
}
#pragma endregion

#pragma region item
void UCharacterInventoryComponent::AddItemInventory(AItemBase* const NewItemBase)
{
	if (ItemArray.Find(NewItemBase) == INDEX_NONE)
	{
		ItemArray.Add(NewItemBase);
	}
}

void UCharacterInventoryComponent::RemoveItemInventory(AItemBase* const ItemBase)
{
	if (ItemArray.Find(ItemBase) != INDEX_NONE)
	{
		ItemArray.Remove(ItemBase);
	}
}

void UCharacterInventoryComponent::ClearItemInventory()
{
	ItemArray.Empty();
}

const TArray<AItemBase*>& UCharacterInventoryComponent::GetItemInventory()
{
	return ItemArray;
}

TArray<AItemBase*> UCharacterInventoryComponent::GetItemInventoryOriginal()
{
	return ItemArray;
}

bool UCharacterInventoryComponent::HasInventoryItems() const
{
	return ArrayExtension::NullOrEmpty(ItemArray);
}
#pragma endregion

void UCharacterInventoryComponent::RemoveAllInventory()
{
	if (!ArrayExtension::NullOrEmpty(WeaponArray))
	{
		for (AWeaponBase*& Weapon : WeaponArray)
		{
			if (Weapon && Weapon->IsValidLowLevel())
			{
				Weapon->Destroy();
				Weapon->ConditionalBeginDestroy();
				Weapon = nullptr;
			}
		}
		WeaponArray.Empty();
	}

	if (!ArrayExtension::NullOrEmpty(ItemArray))
	{
		for (AItemBase*& Item : ItemArray)
		{
			if (Item && Item->IsValidLowLevel())
			{
				Item->Destroy();
				Item->ConditionalBeginDestroy();
				Item = nullptr;
			}
		}
		ItemArray.Empty();
	}
}
