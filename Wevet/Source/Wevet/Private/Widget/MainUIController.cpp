// Copyright © 2018 wevet works All Rights Reserved.

#include "Widget/MainUIController.h"
#include "Engine.h"


UMainUIController::UMainUIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrevWeaponIndex = INDEX_NONE;
	CharacterOwner = nullptr;
	BasePanel = nullptr;
	PlayerHealth = nullptr;
	WeaponFocus = nullptr;

	bWasEquipWeapon = false;
	bTickEventWhenPaused = false;

	/* RootWidget */
	BasePanelKeyName = (TEXT("BasePanel"));
	
	/* PlayerHealth */
	PlayerHealthKeyName = (TEXT("PlayerHealth"));

	/* WeaponFocus */
	WeaponFocusKeyName = (TEXT("WeaponFocus"));

	// @TODO
	/* WeaponWindow */
	WeaponWindowKeyName = (TEXT("WeaponWindow"));
}

void UMainUIController::NativeConstruct()
{
	Super::NativeConstruct();

	BasePanel = Cast<UCanvasPanel>(GetWidgetFromName(BasePanelKeyName));
	WeaponFocus = Cast<UWeaponFocus>(GetWidgetFromName(WeaponFocusKeyName));

	if (WeaponFocus)
	{
		WeaponFocus->Visibility(false);
	}
}

void UMainUIController::Initializer(ACharacterBase* const NewCharacter)
{
	CharacterOwner = Cast<AMockCharacter>(NewCharacter);
	PlayerHealth = Cast<UPlayerHealth>(GetWidgetFromName(PlayerHealthKeyName));

	if (PlayerHealth)
	{
		PlayerHealth->Initializer(CharacterOwner);
	}
}

void UMainUIController::Renderer(float InDeltaTime)
{
	if (!CharacterOwner || !PlayerHealth || !WeaponWindow)
	{
		return;
	}

	VisibleWeaponFocus();
	PlayerHealth->Renderer(InDeltaTime);

	if (CharacterOwner->GetInventoryComponent()->EmptyWeaponInventory())
	{
		WeaponWindow->Visibility(false);
	}
	else
	{
		const int32 WeaponIndex = CharacterOwner->GetSelectWeaponIndex();
		WeaponWindow->RendererImage(CharacterOwner->GetInventoryComponent()->FindByIndexWeapon(WeaponIndex));
		WeaponWindow->RendererAmmos(CharacterOwner->GetInventoryComponent()->FindByIndexWeapon(WeaponIndex));
		WeaponWindow->Visibility(true);
	}
}

void UMainUIController::VisibleWeaponFocus()
{
	if (!WeaponFocus)
	{
		return;
	}

	const bool bWeapon = (CharacterOwner->HasEquipWeapon());
	if (bWeapon == bWasEquipWeapon)
	{
		return;
	}

	bWasEquipWeapon = bWeapon;
	WeaponFocus->Visibility(bWasEquipWeapon);
}

void UMainUIController::SetTickableWhenPaused()
{
	UWorld* const World = GEngine->GameViewport->GetWorld();
	if (World)
	{
		bTickEventWhenPaused = !bTickEventWhenPaused;
		UGameplayStatics::SetGamePaused(World, bTickEventWhenPaused);
		UE_LOG(LogWevetClient, Log, TEXT("bTickEventWhenPaused : %s"), bTickEventWhenPaused ? TEXT("true") : TEXT("false"));
	}
}

void UMainUIController::CreateWeaponWindow(UUniformGridPanel* GridPanel)
{
	if (WindowTemplate)
	{
		WeaponWindow = CreateWidget<UWeaponWindow>(this, WindowTemplate);
		WeaponWindow->Visibility(true);
		GridPanel->AddChildToUniformGrid(WeaponWindow);
	}
}
