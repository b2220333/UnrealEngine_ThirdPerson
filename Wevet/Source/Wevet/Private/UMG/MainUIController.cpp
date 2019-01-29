// Copyright © 2018 wevet works All Rights Reserved.

#include "MainUIController.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "WidgetBlueprintLibrary.h"

UMainUIController::UMainUIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	CharacterOwner(nullptr),
	BasePanel(nullptr),
	FocusPanel(nullptr),
	RadialProgressImage(nullptr),
	BasePanelKeyName(TEXT("BasePanel")),
	FocusPanelKeyName(TEXT("FocusPanel")),
	WeaponGridPanelKeyName(TEXT("WeaponGridPanel")),
	RadialProgressImageKeyName(TEXT("RadialProgressImage")),
	WeaponItemImageKeyName(TEXT("WeaponItemImage")),
	HealthScalarParameterValueName(TEXT("FillAmount")),
	CurrentAmmoKeyName(TEXT("CurrentAmmo")),
	MaxAmmoKeyName(TEXT("MaxAmmo")),
	SpaceAmmoKeyName(TEXT("SpaceAmmo")),
	AmmoCounterBoxKeyName(TEXT("AmmoCounter")),
	bHasWeapon(false)
{
}

void UMainUIController::NativeConstruct()
{
	Super::NativeConstruct();
	BasePanel = Cast<UCanvasPanel>(GetWidgetFromName(BasePanelKeyName));
	FocusPanel = Cast<UCanvasPanel>(GetWidgetFromName(FocusPanelKeyName));
	WeaponGridPanel = Cast<UUniformGridPanel>(GetWidgetFromName(WeaponGridPanelKeyName));
	RadialProgressImage = Cast<UImage>(GetWidgetFromName(RadialProgressImageKeyName));

	for (int index = 0; index < WeaponGridPanel->GetChildrenCount(); ++index)
	{
		if (UWidget* Widget = WeaponGridPanel->GetChildAt(index))
		{
			if (UImage* Image = Cast<UImage>(Widget))
			{
				if (Image->GetName() == WeaponItemImageKeyName.ToString())
				{
					WeaponItemImage = Image;
				}
			}
			if (UHorizontalBox* Box = Cast<UHorizontalBox>(Widget))
			{
				if (Box->GetName() == AmmoCounterBoxKeyName.ToString())
				{
					AmmoCounterBox = Box;
				}
			}
		}
	}

	for (int index = 0; index < AmmoCounterBox->GetChildrenCount(); ++index)
	{
		if (UWidget* Widget = AmmoCounterBox->GetChildAt(index))
		{
			if (UTextBlock* Text = Cast<UTextBlock>(Widget))
			{
				if (Text->GetName() == CurrentAmmoKeyName.ToString())
				{
					CurrentAmmoText = Text;
				}
				else if (Text->GetName() == SpaceAmmoKeyName.ToString())
				{
					SpaceAmmoText = Text;
				}
				else if (Text->GetName() == MaxAmmoKeyName.ToString())
				{
					MaxAmmoText = Text;
				}
			}
		}
	}

	check(
		BasePanel &&
		RadialProgressImage &&
		FocusPanel &&
		WeaponGridPanel &&
		WeaponItemImage &&
		AmmoCounterBox &&
		CurrentAmmoText &&
		MaxAmmoText &&
		SpaceAmmoText);

	WeaponGridPanel->SetVisibility(ESlateVisibility::Hidden);
	WeaponItemImage->SetVisibility(ESlateVisibility::Hidden);
	FocusPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UMainUIController::Initializer(ACharacterBase* const NewCharacter)
{
	CharacterOwner = NewCharacter;
}

void UMainUIController::Renderer(float InDeltaTime)
{
	if (CharacterOwner == nullptr)
	{
		return;
	}
	SetHealth();
	SetVisibilityWeapon();
	SetVisibilityWeaponDetail();
}

//void UMainUIController::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//
//	if (CharacterOwner == nullptr)
//	{
//		return;
//	}
//
//}

void UMainUIController::SetHealth()
{
	if (RadialProgressImage == nullptr)
	{
		return;
	}
	const float Offset = 0.01f;
	const float Health = ICombatExecuter::Execute_IsDeath(CharacterOwner) ? 0.0f : (CharacterOwner->GetHealthToWidget() + Offset);
	if (UMaterialInstanceDynamic* Material = RadialProgressImage->GetDynamicMaterial())
	{
		Material->SetScalarParameterValue(HealthScalarParameterValueName, Health);
	}
	const FSlateBrush Brush = RadialProgressImage->Brush;
	RadialProgressImage->SetBrush(Brush);
}

void UMainUIController::SetVisibilityWeapon()
{
	if (FocusPanel == nullptr || 
		WeaponGridPanel == nullptr || 
		WeaponItemImage == nullptr)
	{
		return;
	}

	const bool bWeapon = (CharacterOwner->GetSelectedWeapon() != nullptr);
	if (bWeapon == bHasWeapon)
	{
		// same state
		return;
	}
	bHasWeapon = bWeapon;
	WeaponGridPanel->SetVisibility(bHasWeapon ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	WeaponItemImage->SetVisibility(bHasWeapon ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	FocusPanel->SetVisibility(bHasWeapon ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMainUIController::SetVisibilityWeaponDetail()
{
	if (!bHasWeapon || 
		WeaponItemImage == nullptr || 
		CurrentAmmoText == nullptr || 
		SpaceAmmoText == nullptr || 
		MaxAmmoText == nullptr)
	{
		return;
	}
	check(CharacterOwner->GetSelectedWeapon());

	const FWeaponItemInfo& WeaponItemInfo = CharacterOwner->GetSelectedWeapon()->WeaponItemInfo;
	const int32 W = 256;
	const int32 H = 194;
	FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(
		WeaponItemInfo.Texture, W, H);

	WeaponItemImage->SetBrush(Brush);
	CurrentAmmoText->SetText(FText::FromString(FString::FromInt(WeaponItemInfo.CurrentAmmo)));
	SpaceAmmoText->SetText(FText::FromString(TEXT("/")));
	MaxAmmoText->SetText(FText::FromString(FString::FromInt(WeaponItemInfo.MaxAmmo)));
}
