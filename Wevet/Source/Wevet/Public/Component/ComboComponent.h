// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEVET_API UComboComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UComboComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetComboPeroid(const bool NewEnableComboPeroid);
	bool GetComboPeroid() const;

	void SetAnyComboActionKeyPressed(const bool NewAnyComboActionKeyPressed);
	bool GetAnyComboActionKeyPressed() const;

	void ResetChainCombo();
	void InitChainCombo();

	void OnChainStrikePressed(bool& OutAnyKeyPressedSuccess);
	void ValidChainCombo(const int32 ArrayNum);

	bool CanNextChain() const;
	void UpdateChainComboIndex();

	int32 GetChainComboIndex() const;

	void HandledChainComboUpdateIndex(const bool bWasMoving, const int32 UpperComboNum, const int32 FullBodyComboNum);
	void DrawLog();
	bool IsValidChainComboIndex() const;

protected:
	bool bEnableComboPeroid;
	bool bAnyComboActionKeyPressed;
	int32 ChainComboIndex;
	int32 PrevChainComboIndex;
};
